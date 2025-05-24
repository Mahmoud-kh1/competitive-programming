random_device rd;
mt19937 mt(rd());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(mt); }
