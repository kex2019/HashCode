#include <bits/stdc++.h>
typedef long long ll;
typedef unsigned int uint;
using namespace std;

//is_vertical, n_tags, tags
typedef tuple<bool, int, set<int>> photo;


unordered_map<string, int> tag_value;
unordered_map<int, string> tag_string;
vector<int> tag_usages;
int N = 0;
int max_tag = 0;
int n_shared_tags = 0;
int n_verticals = 0;
vector<photo> doIO() {
    cin >> N;
    vector<photo> photos(N);
    for(int i = 0; i < N; i++) {
        char type;
        int n;
        cin >> type >> n;
        bool vertical = type == 'V';
        if(vertical) {
            n_verticals++;
        }
        set<int> tags;
        for(int j = 0; j < n; j++) {
            string tag;
            cin >> tag;
            if(tag_value.find(tag) == tag_value.end()) {
                tag_value[tag] = max_tag++;
                tag_string[max_tag-1] = tag;
                tag_usages.push_back(0);
            } else {
                n_shared_tags++;
            }
            int tid = tag_value[tag];
            tag_usages[tid]++;
            tags.insert(tid);
        }

        photos[i] = tuple<bool, int, set<int>>(vertical, n, tags);
    }
    return photos;
}

int main() {
    ios::sync_with_stdio(false);
    vector<photo> photos = doIO();
    cout << "N_PICS: " << N << endl;
    cout << "N_VERTICALS: " << n_verticals << endl;
    cout << "N_TAGS: " << max_tag << endl;
    cout << "N_SHARED_TAGS: " << n_shared_tags << endl;
    cout << endl;
    cout << "TAG usages" << endl;
    int n_2 = 0, n_1 = 0;
    vector<int> n_used_tags(100000);
    for(int i = 0; i < max_tag; i++) {
        n_used_tags[tag_usages[i]] ++;
/*        if(tag_usages[i] == 1) {
            n_1++;
        }
        if(tag_usages[i] == 2) {
            n_2++;
        }*/
//        if(tag_usages[i] != 1 && tag_usages[i] != 2)
//            cout << tag_string[i] << ": " << tag_usages[i] << endl;
    }
    for(int i = 0; i < 100000; i++) {
        if(n_used_tags[i])
            cout << "Used: " << i << " times: " << n_used_tags[i] << endl;
    }
}