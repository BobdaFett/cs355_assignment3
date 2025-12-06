This file contains a brief description of the tests that I've come up with for the Sub class.
It will go in order of definition from main.cpp.

This program does not have any deficiencies that I am aware of, however, I was not sure if using a vector<Sub*>
was the correct way to keep track of the currently running Subs. In lieu of the ban on searching through the
stack and cross-referencing pushed Sub names, I decided that a vector<Sub*> was the cleanest solution.

1) test_call_non_main()
    One of the rules for the Sub class is that the first called subprogram must be named "main_". This test defines two
    Subs - one named "main_" and one named "sub_". It then calls "sub_" as the first subprogram. This should fail, since
    "main_" is the first subprogram to be called.

2) test_equal_depths()
    Static scoping allows functions that are at the same static depth and that share the same static parent to call each
    other. This test defines three Subs. The first is the "main_" function, and the others are at static depth 1. They
    are direct children of "main_", and they should be able to call each other.

3) test_caller_lower_depth()
    Static scoping allows parent functions to call their children. This test defines three Subs. The first is the "main_"
    function. The second, named "sub1", is a child of "main_", and the third, named "sub2", is a child of "sub1". Sub
    "sub1" will then call "sub2". This should succeed, since "sub2" is a child of "sub1".

4) test_caller_higher_depth()
    Static scoping allows child functions to call their static parent's neighbors. This test defines four Subs. The first
    is the "main_" function, as usual. The second and third, "sub1" and "sub2", are children of "main_", respectively.
    Finally, "sub3" is a child of "sub2". "sub3" will then call "sub1". This should succeed, since "sub1" is both a child
    of "main_" and a neighbor to "sub2", which is the static parent of "sub3".

5) test_missing_depth()
    Following the previous two tests, we cannot have a Sub call another Sub's children. This test defines all the same
    Subs as the previous test, but calls them differently. "sub1" will attempt to call "sub3", which is a child of "sub2".
    This should fail.

6) test_return_depth()
    Functions should only be able to return in the order that they were called. This simulated language doesn't allow
    async calls. This test defines and calls all the same Subs as test 4 in the same order. It then calls the "return_"
    method on each Sub in the exact inverse order. This is valid and should complete successfully.

7) test_return_depth_error()
    This test is the same as test 6, except that it calls the "return_" method on the Subs in the wrong order. This
    should fail.

8) test_args_call_return()
    The stack and display should be able to function properly even if a Sub has arguments or local variables. This test
    defines three Subs. The first is the "main_" function, and the others, "sub1" and "sub2", are children of "main_".
    "main_" has no arguments or locals, "sub1" has 1 argument and 2 locals, and "sub2" has 4 arguments and 10 locals.
    This test thoroughly exercises the stack and display to ensure that they are updated properly. It calls "main_",
    then "sub1", then "sub2", and returns in the reverse order. The logged stack and display should line up properly.