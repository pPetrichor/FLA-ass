; Input: a string of 1's and x' =', e.g. '11x111=111111'
;整体的处理思路就是构造一个三纸带图灵机，将x前的内容留在第一个纸带上，x与=之间的内容写到第二个纸带上，=后的内容写到第三个纸带上
;在accept情况下，第三个纸带上'1'的数目是前两个纸带上'1'的数目的乘积，因此构造以下过程模拟乘法：
;首先比较第一个纸带与第三个纸带的内容：如果二者的head均为1，则将第三个纸带的'1'写为空格符，然后两个head右移；这样移动直至第一个纸带的head移动到空格符
;上面的过程相当于模拟了第一个纸带内容x1的过程，因此接着要把第一个纸带的head移动回开头，并将第二个纸带上head指向的1改写为空格符然后右移，表示第二个乘数消耗了1
;这样处理后，重复“比较第一个纸带与第三个纸带内容”这一过程，模拟第一个纸带内容x2的过程
;记纸带一上有a个'1'，纸带二上有b个'1'；重复上述操作，就可以模拟axb的过程
;如果输入满足要求，则处理到最后当纸带二上的内容消耗完后，纸带三也会被清空，此时纸带一的head指向开头，所以这种情况下需要将纸带一内容clear后写上'true'
;reject分为两种情况：如果在将input中相应的内容写到三个纸带上的过程中发现输入不合格式：缺少或者多出x、=，或是三个应为'1'的部分有空内容，则reject
;如果input符合规范，则在模拟乘法的过程中如果遇到正常状态以外的情况，就说明应该reject，详细见State check的解释

; the finite set of states
#Q = {move1_0,move1,move2_0,move2,move3_0,move3,check,tape1_back,OK,return_reject,return,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {1,x,=}

; the complete set of tape symbols
#G = {1,x,=,c,_,t,r,u,e,f,a,l,s} ; c is used to signify a place that be copied

; the start state
#q0 = move1_0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions
; State move1_0: start state
move1_0 1__ 1__ r** move1
move1_0 x__ x__ *** reject
move1_0 =__ =__ *** reject
move1_0 ___ ___ *** reject ; empty input

; State move1: 将tape1的head右移直到首次遇到'x'
move1 1__ 1__ r** move1
move1 x__ c__ r** move2_0
move1 =__ =__ *** return_reject
move1 ___ ___ l** return_reject

; State move2_0: tape1的head刚刚遍历完了'x'，接下来必须是'1'，否则reject
move2_0 1__ c1_ rr* move2
move2_0 x__ x__ *** return_reject
move2_0 =__ =__ *** return_reject
move2_0 ___ ___ l** return_reject

; State move2: 正在处理'x'与'='之间的'1'，要把这些'1'写到tape2上，tape1相应的位置写为'c'；如果遍历到'='则说明该部分处理完毕；没遇到'='或遇到'x'说明input不合规范，reject
move2 1__ c1_ rr* move2
move2 x__ x__ *** return_reject
move2 =__ c__ rl* move3_0
move2 ___ ___ ll* return_reject

; State move3_0: tape1的head刚刚遍历完了'='，接下来必须是'1'，否则reject
move3_0 11_ c11 r*r move3
move3_0 x1_ x1_ *** return_reject
move3_0 =1_ =1_ *** return_reject
move3_0 _1_ _1_ l** return_reject

; State move3: 正在处理'='之后的'1'，需要将这些'1'写到tape3上，tape1相应的位置写为'c'；如果遍历到空格符则说明该部分处理完毕；如果遇到'='或'x'说明input不合规范，reject
move3 11_ c11 r*r move3
move3 x1_ x1_ **l return_reject
move3 =1_ =1_ **l return_reject
move3 _1_ _1_ l*l return

; State return: 将三个纸带的head移动到最左边，同时将tape1的'c'换成空格符；之后进入check进行乘法模拟
return c11 _11 l** return
return c1_ _1_ l** return
return c_1 __1 l** return
return c__ ___ l** return
return 111 111 lll return
return _11 _11 *ll return
return 1_1 1_1 l*l return
return __1 __1 **l return
return 11_ 11_ ll* return
return _1_ _1_ *l* return
return 1__ 1__ l** return
return ___ ___ rrr check

; State check: 进行乘法模拟，每次循环都是移动tape1的head并置空tape3的'1'，每次循环结束后tape1的head移到开头，tape2的head右移一位
;某次循环结束后如果tape2、tape3的head均指向空格符，说明满足相乘的数量关系，则进入OK，accpet
check 111 11_ r*r check
check _11 __1 lr* tape1_back
check _1_ ___ lr* tape1_back
check 1__ ___ r** OK
check 1_1 1_1 *** reject
check 11_ 11_ *** return_reject

; State tape1_back:用于在check时每次循环结束后将tape1的head移回到起点；回到起点后返回check
tape1_back 111 111 l** tape1_back
tape1_back _11 _11 r** check
tape1_back 1_1 1_1 l** tape1_back
tape1_back __1 __1 r** check
tape1_back 11_ 11_ l** tape1_back
tape1_back _1_ _1_ r** check
tape1_back 1__ 1__ l** tape1_back
tape1_back ___ ___ r** check

; State OK:将tape1的内容清空并写下'true'
OK 1__ ___ r** OK
OK ___ ___ *** accept

; State accept*: write 'true' on 1st tape
accept ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; State return_reject: 将三个纸带的head均移动到最左端，之后进入reject来将所有纸带内容清空并写下'false'
return_reject 111 111 lll return_reject
return_reject _11 _11 *ll return_reject
return_reject 1_1 1_1 l*l return_reject
return_reject __1 __1 **l return_reject
return_reject 11_ 11_ ll* return_reject
return_reject _1_ _1_ *l* return_reject
return_reject 1__ 1__ l** return_reject
return_reject ___ ___ rrr reject
return_reject c11 c11 lll return_reject
return_reject c1_ c1_ ll* return_reject
return_reject c_1 c_1 l*l return_reject
return_reject c__ c__ l** return_reject
return_reject x11 x11 lll return_reject
return_reject x1_ x1_ ll* return_reject
return_reject x_1 x_1 l*l return_reject
return_reject x__ x__ l** return_reject
return_reject =11 =11 lll return_reject
return_reject =1_ =1_ ll* return_reject
return_reject =_1 =_1 l*l return_reject
return_reject =__ =__ l** return_reject

; State reject*: 将三个纸带上的head之后的内容全部清除，之后在第一个纸带上写'false'
reject c11 ___ rrr reject
reject c1_ ___ rr* reject
reject c_1 ___ r*r reject
reject c__ ___ r** reject
reject x11 ___ rrr reject
reject x1_ ___ rr* reject
reject x_1 ___ r*r reject
reject x__ ___ r** reject
reject =11 ___ rrr reject
reject =1_ ___ rr* reject
reject =_1 ___ r*r reject
reject =__ ___ r** reject
reject 111 ___ rrr reject
reject _11 ___ *rr reject
reject 1_1 ___ r*r reject
reject __1 ___ **r reject
reject 11_ ___ rr* reject
reject _1_ ___ *r* reject
reject 1__ ___ r** reject
reject ___ f__ r** reject2
reject2 ___ a__ r** reject3
reject3 ___ l__ r** reject4
reject4 ___ s__ r** reject5
reject5 ___ e__ *** halt_reject