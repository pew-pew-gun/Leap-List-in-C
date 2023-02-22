# Leap-List-in-C

Implemented a probabilistic data structure called a 'leap list'. Essentially works in the same way as how, for the same train line, there are express trains that stop at only a few number of stations and ordinary trains that stop at every single station along the line. Our leap list will behave in the same way, we will store multiple sorted singly linked lists, so at the base level, we keep all our items in a sorted singly linked list. Then we do not store every item in the original list but provide long range pointers and store only a few of them (again these items are still sorted as they just leave out some elements from the base list).

How do we select the items in the second list that we wish to keep?
We choose them randomly, and we will let p denote the probability of keeping an item. Then we can generalise further and have a third list that keeps even less and less items, will the same probability p of drawing items from the second list. Each progressive list will be a proper subset of a list on a lower level.

> problem1a.c contains the driver code to populate and insert elements into a leap list and to then query certain values by performing a search.

Input:
The input to the program is delivered on stdin, it follows the following pattern:
<random seed>
<length> <query length>
<height> <p>
<elements>
<query elements>

<random seed> is the number used to initialise the random number generation.
<length> is the number of elements to be read in.
<query length> is the number of elements which will be searched.
<height> is the maximum height of the leap list.
<p> is the probability of a particular element found on one level to be found on the next.
<elements> is a space separated list of numbers comprising the leap list.
<query elements> is a space separated list of numbers comprising the keys which will be searched for.

Output:
The output from the program should be one line per query element, matching the following pattern:
<query element> (<found>): <base accesses> <required accesses>

<query element> is the element being searched for.
<found> is whether the element was found ("present") or not ("not found").
<base accesses> is the position of the element being searched for in the base level of the leap list - treating the roots of the leap list as the 0th element. This could also be thought of as the number of pointer accesses which would be required to reach the element in a regular linked list (including the initial head pointer access).
<required accesses> is the number of pointer accesses required to find the element in the leap list. 

> problem1b.c contains the driver code to populate a leap list, then deletes given elements from that leap list, then queries certain values by searching the resultant leap list.

Input:
The input to the program is delivered on stdin, it follows the following pattern:
<random seed>
<length> <query length>
<height> <p>
<elements>
<query elements>
<delete length>
<delete elements>

<delete length> is the number of elements which will be deleted from the list.
<delete elements> is a space separated list of numbers comprising the keys which will be deleted.
The remaining components of the input are the same when executing problem1a.c.

Output:
The output from the program will be comprised of two components, the first is a list of keys occurring at each level, that is, the resulting leap list after deletion will be printed out. The second component will be the same as in the execution of problem1a.c, a set of results of the queries performed on the list.

The source code for the files: problem1a.c, problem1b.c, and utils.c and skeleton code have been graciously provided by Grady Fitzpatrick of the University of Melbourne.
