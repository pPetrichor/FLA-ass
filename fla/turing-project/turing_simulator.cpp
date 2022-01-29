#include "turing.h"
int TM::check_input(string input)//return 1st illegal char if exits, else return -1
{
    for(int i=0;i<input.length();++i)
    {
        int j=0;
        for(;j<S.size();++j)
        {
            if(input[i] == S[j]) break;
        }
        if(j == S.size()) //illegal
            return i;
    }
    return -1;
}
void TM::run(string input,bool whether_v)
{
    //check if input is legal
    int loc = check_input(input);
    if(loc != -1)
    {
        if(!whether_v)
            cerr << "illegal input" << endl;
        else
        {
            cerr << "Input: " << input << endl << "==================== ERR ====================" << endl;
            cerr << "error: \'" << input[loc] << "\' was not declared in the set of input symbols" << endl;
            cerr << "Input: " << input << endl;
            for(int i=0;i<loc + 7;++i) cerr << ' ';
            cerr << '^' << endl;
            cerr << "==================== END ====================" << endl;
        }
        normal = false;
        return;
    }
    //start running
    if(whether_v)
        cout << "Input: " << input << endl << "==================== RUN ====================" << endl;
    if(input == "") input += B;
    get_start_id(input,whether_v);
    while(1)
    {
        bool f = false;
        for(int i=0; i<F.size();++i)
        {
            if(F[i] == current_id.state)
                {f = true;break;}
        }
        if(f) break;
        current_id.step++;
        trans_way current_trans;
        f = search_trans(current_trans);
        if(!f) break;
        else
        {
            current_id.state = current_trans.new_state;
            for(int i = 0;i < n; ++i)
            {
                current_id.tape[i][current_id.head[i]] = current_trans.rewrite_symbols[i];
                if(current_trans.move[i] == 'l')
                {
                    current_id.head[i]--;
                    if(current_id.head[i] < 0)
                    {
                        current_id.index[i]--;
                        current_id.tape[i] = B + current_id.tape[i];
                        current_id.head[i]++;
                    }
                }
                else if (current_trans.move[i] == 'r')
                {
                    current_id.head[i]++;
                    if(current_id.head[i] >= current_id.tape[i].length()) current_id.tape[i] += B;
                }
                int left = 0,right = current_id.tape[i].length()-1;
                while(current_id.tape[i][left] == B) left++;
                if(left > current_id.head[i]) left = current_id.head[i];
                while(current_id.tape[i][right] == B) right--;
                if(right < current_id.head[i]) right = current_id.head[i];
                string temp;
                for(int l = left;l <= right;l++)
                    temp += current_id.tape[i][l];
                
                current_id.tape[i].clear();
                current_id.tape[i] += temp;
                while(left > 0) 
                {
                    current_id.index[i]++;
                    current_id.head[i]--;
                    left--;
                }
            }
            
            if(whether_v) show_id();
        }
    }
    if(whether_v) cout << "Result: ";
    int i = 0;
    while(current_id.tape[0][i] == B && i < current_id.tape[0].length()) i++;
    while(current_id.tape[0][i] != B && i < current_id.tape[0].length())
        {cout << current_id.tape[0][i];i++;}
    cout << endl;
    if(whether_v) cout << "==================== END ====================" << endl;
    delete[] current_id.index;
    delete[] current_id.head;
    delete[] current_id.tape;
}
bool TM::search_trans(trans_way &x)
{
    string head_content = "";
    for(int i=0;i<n;++i)
    {
        head_content += current_id.tape[i][current_id.head[i]];
    }
    pair<string,string> temp = make_pair(current_id.state,head_content);
    map<pair<string,string>,trans_way> ::iterator it;
    it = trans.deltas.find(temp);
    if(it != trans.deltas.end())
    {
        x = it->second;
        return true;
    }
    else
    {
        return false;
    }
    
    // map<pair<string,string>,trans_way> ::iterator it;
    // for(it = trans.deltas.begin();it!=trans.deltas.end();++it)
    // {
    //     if(it->first.first == current_id.state)
    //     {
    //         string temp = it->first.second;
    //         bool f = true;
    //         for(int i=0;i<n;++i)
    //         {
    //             if(current_id.tape[i][current_id.head[i]] != temp[i])
    //             {
    //                 f = false;break;
    //             }
    //         }
    //         if(f)
    //         {
    //             x = it->second;
    //             return true;
    //         }
    //     }
    // }
    // return false;
}
void TM::get_start_id(string input,bool whether_v)
{
    current_id.step = 0;
    current_id.index = new int[n];
    current_id.state = q_0;
    current_id.head = new int[n];
    current_id.tape = new string[n];
    current_id.tape[0] = input;
    for(int i=0;i<n;++i) {current_id.index[i] = 0;current_id.head[i] = 0;}
    for(int i=1;i<n;++i) current_id.tape[i] += B;
    if(whether_v)
        show_id();
}
int le(int n)
{
    if(n == 0) return 1;
    int res = 0;
    while(n > 0)
    {
        res++;
        n /= 10;
    }
    return res;
}
void TM::show_id()
{
    cout << "Step   : " << current_id.step << endl;
    for(int i=0;i<n;++i)
    {
        int max = current_id.tape[i].length();
        int *blanks = new int[max];
        blanks[0] = 0;
        cout << "Index" << i << " : ";
        int j = 0;int t = 0;
        for(j=current_id.index[i];j < max + current_id.index[i] -1;++j)
        {
            t = (j < 0) ? (-1*j) : j;
            cout << t << ' ';
            int n = le(t);
            blanks[j - current_id.index[i] + 1] = n;
        }

        t = (j < 0) ? (-1*j) : j;
        cout << t << endl;
        cout << "Tape" << i << "  : ";
        for(j=0;j<max-1;++j)
        {
            for(int l=0;l<blanks[j];++l)
                cout << ' ';
            if(j<current_id.tape[i].length())
                cout << current_id.tape[i][j];
            else cout << B;
        }
        for(int l=0;l<blanks[j];++l)
            cout << ' ';
        if(j<current_id.tape[i].length())
            cout << current_id.tape[i][j] << endl;
        else cout << B << endl;
        cout << "Head" << i << "  : ";
        
        for(j=0;j<current_id.head[i];++j)
        {
            for(int t=0;t<blanks[j];++t)
                cout << ' ';
            cout << ' ';
        }
        for(int l=0;l<blanks[j];++l)
            cout << ' ';
        cout << '^' << endl;
        delete[] blanks;
    }
    cout << "State  : " << current_id.state << endl;
    cout << "---------------------------------------------" << endl;
}