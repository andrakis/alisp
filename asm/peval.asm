;; Patterned Evaluator
;; Based upon the SchemingSharply CellMachine evaluator and the abandoned pattern
;; evaluator in alisp.
;;
;; Implements the ALisp evaluator with the goal to maximise speed and minimize
;; the number of operations required for evaluation.
;; This goal is expected to be acheived by:
;;  o A simple virtual machine utilizing two stacks (an integer stack and a cell
;;    stack.)
;;  o Fewer, more complex instructions.
;;  o Jumptables, which act like a C-style switch statement, instead of many
;;    comparison calls for determining how to proceed with evaluation.
;;  o Integers used for core registers (CellMachine used cells even for these)
;;  o Special registers for cell arguments.
;;  o Using the cell stack for stack variables, as well as arguments.
;;
;; Design notes:
;;   - Virtual machine registers:
;;     o PC   integer   Program counter
;;     o SP   integer   Stack pointer: see IS
;;     o BP   integer   Base pointer: see IS
;;     o A    integer   Accumulator
;;     o CA   cell      Cell accumulator
;;     o CSP  integer   Cell stack pointer: see CS
;;     o CBP  integer   Cell base pointer: see CS
;;     o IS   [integer] Integer Stack. Not directly accessible.
;;     o CS   [cell]    Cell Stack. Not directly accessible.
;;   - Virtual machine stacks:
;;     o Two different types of stacks are used.
;;     o The Integer Stack (IS):
;;       + A simple list of integers.
;;       + Predetermined fixed size (set at VM creation time).
;;       + Initially all values set to 0.
;;       + Only stores integer values, not cells.
;;       + SP and BP keep track of PC inside recursive eval calls.
;;       + PUSH and POP may be used to manipulate this stack.
;;     o The Cell Stack (CS):
;;       + A list of cells.
;;       + Initially contains arguments passed to eval.
;;       + Increases or decreases in size as necessary.
;;       + CSP and CBP keep track of arguments and stack variables.
;;       + CPUSH, CPOP, and CADJUST may be used to manipulate this stack.
;;   - Jumptables
;;     o Jumptables are a cell lists.
;;     o Each item in the list is another list in the form:
;;           [Key, Value]
;;     o The key can be any cell type.
;;     o The Value is usually an integer-type cell, representing a PC
;;       address to jump to.
;;     o Jumptables use Simple List Item Matching (SLIM).
;;     o SLIM reserves one atom, _ (underscore), for use as a wildcard.
;;     o When comparing items, _ matches any item (including lists.)
;;     o Lists are compared recursively, and wildcards may be used inside
;;       lists to denote any match.
;;     o Examples:
;;         Item A        Item B      Match?
;;         1             1           Y
;;         1             2           N
;;         1             _           Y
;;         _             2           Y
;;         [1, 2, 3]     [1, 2, 3]   Y
;;         [1, 3, 2]     [1, 2, 3]   N
;;         [1, _, 3]     [1, 2, 3]   Y
;;         [[1, 2], 3]   [[1, 2], 3] Y
;;         [_, 3]        [[1, 2], 3] Y
;;         [[1, _], _]   [[1, 2], 3] Y
;;       The above examples use only numbers and _, but any comparable cell
;;       value may be used.
;;     o JUMPTABLE instruction:
;;       JUMPTABLE [Modifier] Key, Table
;;     o Available Modifiers:
;;       (none)    Use Key as given
;;       Typeof    Use (typeof Key)
;;       Head      Use the first element in Key

;; Initialisation point.
;; Sets up the following Jumptables:
;;
;; Type jumptable:
;;   Key: Typeof(X)
;;   Values:
;;     atom     eval_getatom   Get the value of atom named in X
;;     list     eval_builtin   Treat as procedure call (builtin or otherwise)
;;     _        eval_asis      Anything else is returned as is
;;
;; Builtin jumptable: builtins that usually recursively call eval
;;   Key: Atom
;;   Values:
;;     quote      eval_builtin_quote       (quote exp)
;;     if         eval_builtin_if          (if test conseq [alt])
;;     define     eval_builtin_define      (define var exp)
;;     set!       eval_builtin_set!        (set! var exp)
;;     lambda     eval_builtin_lambda      (lambda (var*) exp)
;;     macro      eval_builtin_macro       (macro (var*) exp)
;;     fastmacro  eval_builtin_fastmacro   (fastmacro (var*) exp)
;;     begin      eval_builtin_begin       (begin exp*)
;;     next       eval_builtin_next        (next fun args*)
;;     _          eval_proc                Not a builtin, treat as proc
;;
;; Proc jumptable: different execution methods for different types of proc
;;   Key: Typeof(Proc)
;;   Values:
;;     lambda     eval_proc_lambda     Evaluate as lambda
;;     macro      eval_proc_macro      Evaluate as macro, then evaluate result
;;     fastmacro  eval_proc_fastmacro  As above but reuse macro environment
;;     proc       eval_proc_proc       Call a procedure
;;
;; ProcExps jumptable: arguments evaluation (skipped for macros)
;;   Key: Typeof(Proc)
;;   Values:
;;     macro      eval_exps_macro   Do not evaluate arguments
;;     fastmacro  eval_exps_macro   As above
;;     _          eval_exps_other   Evaluate all arguments
Entry:

!local JT_Types, JT_Builtins, JT_Procs, JT_ProcExps
CELLINIT JT_Types LIST
CELLINIT JT_Builtings LIST
CELLINIT JT_Procs LIST
CELLINIT JT_ProcExps LIST


;; Evaluator.
;; ARGS:
;;   X    Item to evaluate
;;   ENV  Environment to use
Eval:
!arguments X, ENV
JUMPTABLE Typeof X, JT_Types

eval_getatom:
ENV:GET X, ENV
RET

eval_builtin:
JUMPTABLE Head X, JT_Builtins

eval_asis:
LOADCA X
RET

;; Procedure evaluation: (Proc Exps*)
;; Evaluates Proc by calling Eval on it.
;; Optionally evaluates all Exps (if not macro or fastmacro).
;; Invokes Proc with Exps depending on type.
eval_proc:
LOADCA X
!local Xh, Exps
HEADTAIL Xh, Exps
CPUSH Xh
CPUSH Env
CALL Eval
STORE Xh
JUMPTABLE Typeof Xh, JT_ProcExps
