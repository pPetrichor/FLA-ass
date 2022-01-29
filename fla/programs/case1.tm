; Input: a string of a's and b's, e.g. 'aabaab'
;整体的处理思路就是将前一半的字符a…ab…b留在第一个纸带上，后面的a…ab…b写到第二个纸带上，然后比较两个纸带上内容是否相同；如果相同则accept，否则reject
;因此，首先遍历input，如果在依次遍历过若干a与b之后又遍历到a，则说明遍历到了后半段字符串，因此就将后面的内容写到第二个纸带
;如果在遍历input的过程中发现前一半字符串不是a…ab…b的形式，则直接reject

; the finite set of states
#Q = {0,1,2,cp,cmp,return_reject,return,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,a,l,s} ; c is used to signify a place that be copied; c用来标记此位置上的input已经被写到了其它纸带上 

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions
; State 0: start state
0 a_ a_ r* 1
0 b_ __ r* reject
0 __ __ ** reject ; empty input

; State 1: 将tape1的head右移直到首次遇到'b'
1 a_ a_ r* 1
1 b_ b_ r* 2
1 __ __ l* return_reject ; 输入为'aa…a'，不接受

; State 2: 将tape1的head右移直到再次遇到'a'
2 a_ a_ ** cp
2 b_ b_ r* 2
2 __ __ l* return_reject ; 输入为'aa…abb…b'，不接受

; State cp: 将tape1的head之后的内容剪切到tape2上
cp a_ ca rr cp
cp b_ cb rr cp
cp __ __ ll return ;剪切完毕，进入return状态返回最左边进行比较

; State return: 将两个纸带的head移动到最左边，之后进入cmp进行比较
return ca _a l* return
return cb _b l* return
return c_ __ l* return
return a_ a_ l* return_reject
return b_ b_ l* return_reject
return _a _a *l return_reject
return _b _b *l return_reject ;四个转移函数说明如果两个纸带上的长度不一样，则必然不接受
return aa aa ll return
return bb bb ll return
return ab ab ll return
return ba ba ll return
return __ __ rr cmp

; State cmp: compare two strings
cmp aa __ rr cmp
cmp bb __ rr cmp
cmp ab __ rr reject
cmp ba __ rr reject
cmp __ __ ** accept ;二者相同，接收

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State return_reject: 将两个纸带的head移动到最左边，之后进入reject
return_reject a_ a_ l* return_reject
return_reject b_ b_ l* return_reject
return_reject _a _a *l return_reject
return_reject _b _b *l return_reject
return_reject aa aa ll return_reject
return_reject bb bb ll return_reject
return_reject ab ab ll return_reject
return_reject ba ba ll return_reject
return_reject __ __ rr reject

; State reject*: 将两个纸带上的head之后的内容全部清除，之后在第一个纸带上写'false'
reject aa __ rr reject
reject ab __ rr reject
reject ba __ rr reject
reject bb __ rr reject
reject a_ __ r* reject
reject _b __ *r reject
reject b_ __ r* reject
reject _a __ *r reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject