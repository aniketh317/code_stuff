struct FenwickTree {
    vector<int> bit;  // binary indexed tree
    int n;

    FenwickTree(int n) {
        this->n = n;
        bit.assign(n, 0);
    }

    FenwickTree(vector<int> const& a) : FenwickTree(a.size()) {
        for (size_t i = 0; i < a.size(); i++)
            update(i, a[i]);
    }

    int max(int r) {
        int ret = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1)
            ret = (ret > bit[r]) ? ret : bit[r];
        return ret;
    }

    void update(int idx, int val) {
        /*Works because there is only one update to idx, which increases the value of the element*/
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] = (bit[idx] > val) ? bit[idx] : val;
    }
};
