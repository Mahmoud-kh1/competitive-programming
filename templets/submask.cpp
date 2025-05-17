dont forget to get all function of bitset it's important for dp problems
for (int mask = 1;  mask < constrins; maks++){
for (int submask = mask; submask; submask = (submask - 1) & mask) {
        submasks.push_back(submask);
    }
}
time o(3 ^ n)
