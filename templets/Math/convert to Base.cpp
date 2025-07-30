using A = array<int, M>;
string divideWithBase(const string &num, int base, int &remain) {
    string quotient;
    quotient.reserve(num.size());
 
    int carry = 0;
    for(char ch: num) {
        int d = ch - '0';
        int current = carry * 10 + d;
        int q = current / base;
        carry = current % base;
        if(not quotient.empty() or q != 0) quotient.push_back(char('0' + q));
    }
 
    remain = carry;
    return quotient.empty()? "0" : quotient;
}
A convertToBase(string n, int base) {
    A res{};
    res.fill(0);
    if(n == "0") return res;
 
    vector<int> result;
    while(n != "0") {
        int remain;
        n = divideWithBase(n, base, remain);
        result.push_back(remain);
    }
 
    for(int i = 0; i < result.size(); ++i) res[i] = result[i];
 
    return res;
}
