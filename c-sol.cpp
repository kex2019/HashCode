#include <bits/stdc++.h>
typedef long long ll;
typedef unsigned int uint;
using namespace std;

//is_vertical, n_tags, tags
typedef tuple<bool, int, set<int>> photo;

int calc_score(const photo& p1, const photo& p2) {
    auto &t1 = get<2>(p1);
    auto &t2 = get<2>(p2);
    int n_common = 0;
    int t1_sc = 0;
    int t2_sc = 0;
    for(int t : t1) {
        if(t2.find(t) != t2.end()) {
            n_common ++;
        } else {
            t1_sc++;
        }
    }
    for(int t : t2) {
        if(t1.find(t) == t1.end()) {
            t2_sc++;
        }
    }

//    int in_t2_not_in_t1 = t2.size() - in_t1_not_in_t2;
    return std::min(std::min(n_common, t1_sc), t2_sc);
}
unordered_map<string, int> tag_value;
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
            } else {
                n_shared_tags++;
            }
            int tid = tag_value[tag];
            tags.insert(tid);
        }

        photos[i] = tuple<bool, int, set<int>>(vertical, n, tags);
    }
    return photos;
}

// New score when we move a to b
int calc_new_score(vector<photo>& photos, int aidx, int bidx) {
    if(bidx == 0) {
        return calc_score(photos[aidx], photos[1]);
    }
    if(bidx == photos.size() - 1) {
        return calc_score(photos[photos.size() - 2], photos[aidx]);
    }
    return calc_score(photos[aidx], photos[bidx+1]) + calc_score(photos[aidx], photos[bidx-1]);
}

pair<vector<pair<int,photo>>, vector<pair<int,photo>>> separate_photos(vector<photo> & photos) {
    vector<pair<int,photo>> verticals;
    verticals.reserve(photos.size());
    vector<pair<int,photo>> horizontals;
    horizontals.reserve(photos.size());
    
    for(int i = 0; i < photos.size(); i++) {
        auto &p = photos[i];
        if(get<0>(p)) {
            verticals.push_back({i, p});
        } else {
            horizontals.push_back({i, p});
        }
    }

    return {verticals, horizontals};
}

// Calc score between aidx and bidx
int calc_double_score(const vector<pair<int,int>> &seq, vector<photo> &photos, int aidx, int bidx) {
    auto a = seq[aidx];
    auto b = seq[bidx];

    auto aft = get<2>(photos[a.first]);
    auto ast = get<2>(photos[a.second]);
    auto bft = get<2>(photos[b.first]);
    auto bst = get<2>(photos[b.second]);    
    set<int> atags;
    set<int> btags;
    atags.insert(aft.begin(), aft.end());
    atags.insert(ast.begin(), ast.end());
    btags.insert(bft.begin(), bft.end());
    btags.insert(bst.begin(), bst.end());
    
    auto &t1 = atags;
    auto &t2 = btags;
    int n_common = 0;
    int t1_sc = 0;
    int t2_sc = 0;
    for(int t : t1) {
        if(t2.find(t) != t2.end()) {
            n_common ++;
        } else {
            t1_sc++;
        }
    }
    for(int t : t2) {
        if(t1.find(t) == t1.end()) {
            t2_sc++;
        }
    }

    return std::min(std::min(n_common, t1_sc), t2_sc);
}

int calc_move_score(vector<pair<int,int>> &seq, vector<photo> &photos, int aidx, int bidx) {
    if(bidx == 0) {
        return calc_double_score(seq, photos, aidx, 1);
    }
    if(bidx == photos.size() - 1) {
        return calc_double_score(seq, photos, photos.size() - 2, aidx);
    }
    return calc_double_score(seq, photos, aidx, bidx+1) + calc_double_score(seq, photos, aidx, bidx-1);

}

set<int> set_union(set<int>& a, set<int>& b) {
    set<int> un;
    un.insert(a.begin(), a.end());
    un.insert(b.begin(), b.end());
    return un;
}

int main() {
    ios::sync_with_stdio(false);
    vector<photo> photos = doIO();
    auto verthorz = separate_photos(photos);
    auto &verticals = verthorz.first;
    auto &horizontals = verthorz.second;
    vector<pair<int,int>> slides;
    for(int i = 0; i < horizontals.size(); i++) {
        slides.emplace_back(i, i);
    }

    for(auto i : verticals) {
        cout << i.first << endl;
    }

    cout << "VERICALS" << endl << endl;

    vector<bool> vert_used(verticals.size());
    for(int i = 0; i < verticals.size(); i++) {
        if(vert_used[i])
            continue;
        vert_used[i] = true;
        int min_size = 1e6;
        int min_idx = -1;
        for(int j = 0; j < verticals.size(); j++) {
            if(vert_used[j])
                continue;
            int size = set_union(get<2>(verticals[i].second), get<2>(verticals[j].second)).size();
            if(size < min_size) {
                min_size = size;
                min_idx = j;
            }
        }
        vert_used[min_idx] = true;
        slides.emplace_back(i, min_idx);
        cout << i << " : " << min_idx << endl;
    }

    vector<vector<int>> pairwise(slides.size(), vector<int>(slides.size()));
    for(int i = 0; i < slides.size(); i++) {
        for(int j = 0; j < slides.size(); j++) {
            int score = calc_double_score(slides, photos, i, j);
            pairwise[i][j] = score;
            pairwise[j][i] = score;
        }
    }
    
    vector<bool> used(slides.size());
    used[0] = true;
    vector<int> sequence{0};
    for(int i = 1; i < slides.size(); i++) {
        int current = sequence.back();
        // Find max for this slide
        int max_score = -1;
        int max_idx = -1;
        for(int j = 0; j < slides.size(); j++) {
            if(used[j]){
                continue;
            }
            
            int score = pairwise[current][j];
            if(score > max_score) {
                max_score = score;
                max_idx = j;
            }
        }
        if(max_idx == -1) {
            break;
        }

        sequence.push_back(max_idx);
        used[max_idx] = true;
    }

/*
    for(int i = 0; i < sequence.size() - 1; i++) {
        max_score += calc_double_score(sequence, photos, i, i + 1);
    }


    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,sequence.size()-1); // distribution in range [1, 6]

    vector<pair<int,int>> max_sequence = sequence;

    int MAX_NEG_SCORE = 100;
    int current_score = max_score;
    vector<pair<int,int>> current_seq = max_sequence;
    for(int i = 0; i < 10000000; i++) {
        int a = dist(rng), b = dist(rng);
        if(a == b) {
            continue;
        }
        
        int cur_score_left = calc_double_score(current_seq, photos, a, a);
        int cur_score_right = calc_double_score(current_seq, photos, b, b);
        int new_score_left = calc_double_score(current_seq, photos, a, b);
        int new_score_right = calc_double_score(current_seq, photos, b, a);
        int diff_score = (new_score_left + new_score_right) - (cur_score_left + cur_score_right);
        //cout << "DIFF SCORE: " << diff_score << endl;
        current_score += diff_score;
        swap(current_seq[a], current_seq[b]);
        if(max_score - current_score > MAX_NEG_SCORE) {
            // Reset to best solution
            current_seq = max_sequence;
            current_score = max_score;
        }

        if(current_score > max_score) {
            max_score = current_score;
            max_sequence = current_seq;
        }
        
    }

    sequence = max_sequence;*/

    //vector<int> sequence = solveB(photos);
    cout << sequence.size() << endl;
    for(auto j : sequence) {
        auto i = slides[j];
        cout << "II: " << i.first << ";;;" << i.second << "===" << j << endl ;
        if(i.first == i.second) {
//            cout << i.first << "\n";
        } else {
//            cout << i.first << " " << i.second << "\n";
        }
    }
    cout << endl;
}