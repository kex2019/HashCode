#include <bits/stdc++.h>
typedef long long ll;
typedef unsigned int uint;
using namespace std;

//is_vertical, n_tags, tags
typedef tuple<bool, int, set<int>> photo;


unordered_map<string, int> tag_value;
unordered_map<int, string> tag_string;
vector<vector<int>> tag_usages;
vector<int> tag_n_uses;
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
                tag_usages.push_back(vector<int>());
                tag_n_uses.push_back(0);
            } else {
                n_shared_tags++;
            }
            int tid = tag_value[tag];
            tag_usages[tid].push_back(i);
            tag_n_uses[tid]++;
            tags.insert(tid);
        }

        photos[i] = tuple<bool, int, set<int>>(vertical, n, tags);
    }
    return photos;
}

int main() {
    ios::sync_with_stdio(false);
    vector<photo> photos = doIO();
    vector<bool> used(N);
    used[0] = true;
    vector<int> sequence{0};
    for(int i = 1; i < N; i++) {
        int current = sequence.back();
        // Find tags that are used twice for this one
        vector<int> twice_used_tags;
        for(int tag : get<2>(photos[current])) {
            if(tag_n_uses[tag] > 1) {
                twice_used_tags.push_back(tag);
            }
        }

        int next = -1;
        // Look for a not used photo to pair this with
        for(int j : twice_used_tags) {
            if(next != -1) {
                break;
            }
            for(int n : tag_usages[j]) {
                if(!used[n]) {
                    next = n;
                    break;
                }
            }
        }

        if(next == -1) {
            for(int j = 0; j < N; j++) {
                if(!used[j]) {
                    next = j;
                }
            }
        }
        used[next] = true;
        sequence.push_back(next);
    }

    cout << sequence.size() << endl;
    for(int i : sequence) {
        cout << i << "\n";
    }
    cout << flush;
}