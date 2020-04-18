#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds; 
using namespace std;

#define BSET(a, p) ((a) | (1ll << (p)))
#define BCHK(a, p) ((a) & (1ll << (p)))
#define BXOR(a, p) ((a) ^ (1ll << (p)));
#define BREM(a, p) (BCHK(a, p)?(BXOR(a, p)):(a))
#define BSHO(a, N) (bitset<N>(a))

#define fi first
#define sc second 
#define pb push_back


#ifdef DEBUG
    #define dbg(s) {s;}
    #define PRELUDE
#endif

#ifndef DEBUG 
    #define PRELUDE { ios :: sync_with_stdio(false); cin.tie(0); cout.tie(0); }
    #define dbg(s)
    #define endl "\n"
#endif

#define int ll
#define i32 int32_t

#define RBTTYPE int
#define ordered_set tree<RBTTYPE, null_type, less<RBTTYPE>, rb_tree_tag,tree_order_statistics_node_update> 
// Ordered set docs:
// order_of_key (k) : Number of items strictly smaller than k .
// find_by_order(k) : K-th element in a set (counting from zero).

#define all(v) (v).begin(),(v).end()

typedef long long ll;
typedef long double ld;
typedef pair< int, int > pii;
typedef pair< ll, ll > pll;

// To remove randomization use 0 below:
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
inline int ri(int x) { // from [0, n-1]
    return uniform_int_distribution<int>(0, x - 1)(rng);
}

inline ld rf() { // from [0, 1]
    return uniform_real_distribution<ld>(0, 1)(rng);
}

ll gcd(ll x, ll y) {
    if (x < y) return gcd(y, x);
    if (y == 0) return x;
    return gcd(y, x % y);
} 

const ll mod = 1e9 + 7;

ll modexp(ll x, ll ex) {
    ll ans = 1ll;
    while (ex > 0) {
        if (ex & 1ll) ans = (ans * x) % mod;
        ex >>= 1ll;
        x = (x * x) % mod;
    }
    return ans;
}

const int maxn = 2e5 + 7;

const ll inf = 1e14 + 7;
const int bsiz = 450;

int n,m;
int arr[maxn];
vector<pair<int,pii>> qu[bsiz];
int sto[maxn];
int ans[maxn];
int ps[maxn];
int suc[maxn];
int pre[maxn];
pair<pii,int> info[maxn];

bool comp(pair<int,pii> p1, pair<int,pii>p2)
{
    return p1.sc.sc > p2.sc.sc;
}

void del(int x)
{
    pre[suc[x]] = pre[x];
    suc[pre[x]] = suc[x];
}

int utilFunc(int a, int b)
{
    if(a==-1 || b==-1)
        return 0;
    return abs(ps[a] - ps[b]);
}

int add(int x)
{
    int ret = 0;
    int s1=-1,s2 = -1;
    if(suc[x]<=n)
        s1 = suc[x];
    if(s1!=-1 && suc[s1]<=n)
        s2 = suc[s1];
    int r1=-1,r2=-1;
    if(pre[x]>0)
        r1 = pre[x];
    if(r1!=-1 && pre[r1]>0)
        r2 = pre[r1];
    ret+= utilFunc(r1,s1);
    ret+= utilFunc(r2,x);
    ret+= utilFunc(s2,x);

    ret-= utilFunc(r2,s1);
    ret-= utilFunc(r1,s2);
    dbg(cout<<"Add"<<" p2,p1,x,s1,s2 "<<r2<<" "<<" "<<r1<<" "<<" "<<x<<" "<<s1<<" "<<s2<<" "<<ret<<endl;)
    pre[suc[x]] = suc[pre[x]] = x;
    return  ret;
}

void setUp()
{
    for (int i = 1; i <= n; ++i)
    {
        pre[i] = i - 1;
        suc[i] = i + 1;
    }
    pre[n + 1] = n;
    suc[0] = 1;
}

void start()
{
    int x;
    sort(info + 1, info + n + 1);
    for (int i = 1; i <= n; ++i) {
        x = info[i].sc;
        arr[x] = i;
        ps[i] = info[i].fi.sc;
    }
}

void solve()
{
    cin>>n;
    for(int i=1;i<=n;i++)
    {
        cin>>info[i].fi.fi;
        cin>>info[i].fi.sc;
        info[i].sc = i;
    }
    cin>>m;
    start();

    dbg(for(int i=1;i<=n;i++) {
        cout << arr[i] << " " << ps[i] << endl;
    })

    int l,r;
    for(int i=1;i<=m;i++)
    {
        cin>>l>>r;
        qu[l/bsiz].pb({i,{l,r}});
    }

    int p,q;
    int val;
    for(int i=0;i<bsiz;i++)
    {
        if(qu[i].empty())
            continue;
        setUp();
        p = min(i * bsiz + bsiz - 1,n), q = i * bsiz;
        if(q==0)
            q++;
        dbg(cout<<"q: "<<q<<" p: "<<p<<endl;)
        if(q>n)
            break;
        for (int j = 1; j < p; ++j) del(arr[j]);
        for (int j = n; j > p; --j) del(arr[j]);
        sto[p] = 0;
        for(int j=p+1;j<=n;j++) sto[j] = sto[j-1] + add(arr[j]);
        dbg(for(int j=p+1;j<=n;j++) cout<<sto[j]<<" "; cout<<endl;)
        for(int j=p-1;j>=q;j--) add(arr[j]);
        sort(all(qu[i]),comp);
        l = q;
        r = n;
        for(auto x:qu[i])
        {
            dbg(cout<<"Query--"<<x.sc.fi<<" "<<x.sc.sc<<endl;)
            while (r > x.sc.sc) {
                del(arr[r]);
                r--;
            }
            if(r>p)
                val = sto[r];
            else
                val = 0;
            dbg(cout<<val<<endl;)
            while (l < p && l<r) {
                del(arr[l]);
                l++;
            }

            while (l > x.sc.fi) {
                l--;
                val = val + add(arr[l]);
                dbg(cout<<"l val"<<l<<" "<<val<<endl;)
            }
            while (l > q) {
                l--;
                add(arr[l]);
            }
            ans[x.fi] = val;
        }
    }

    for(int i=1;i<=m;i++)
        cout<<ans[i]<<endl;

}

i32 main() {
    //freopen("<file>.in", "r", stdin);
    //freopen("<file>.out", "w", stdout);
    
    PRELUDE;
    int t = 1;
    //cin>>t;
    while(t--)
        solve();
}
