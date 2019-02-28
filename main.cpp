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

vector<int> solveB(vector<photo>& photos) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,photos.size()-1); // distribution in range [1, 6]
    int N = photos.size();
    // Generate random solutions
    vector<int> max_sequence;
    int max_score = 0;
    for(int i = 0; i < 20; i++) {
        // Generate random solution
        std::vector<int> idxs(N);
        for(int j = 0; j < N; j++) {
            idxs[j] = j;
        }

        std::random_shuffle(idxs.begin(), idxs.end());
        int score = 0;
        for(int j = 0; j < N - 1; j++) {
            score += calc_score(photos[idxs[j]], photos[idxs[j+1]]);
        }
        if(score > max_score) {
            max_sequence = idxs;
            max_score = score;
        }
    }

    int MAX_NEG_SCORE = 100;
    int current_score = max_score;
    vector<int> current_seq = max_sequence;
    for(int i = 0; i < 1000000; i++) {
        int a = dist(rng), b = dist(rng);
        if(a == b) {
            continue;
        }
        
        int cur_score_left = calc_new_score(photos, a, a);
        int cur_score_right = calc_new_score(photos, b, b);
        int new_score_left = calc_new_score(photos, a, b);
        int new_score_right = calc_new_score(photos, b, a);
        int diff_score = (new_score_left + new_score_right) - (cur_score_left + cur_score_right);
        cout << "DIFF SCORE: " << diff_score << endl;
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

    return max_sequence;
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

int main() {
    ios::sync_with_stdio(false);
    vector<photo> photos = doIO();
/*    vector<int> same_as(N);
    for(int i = 0; i < N / 10; i++) {
        int same = 0;
        for(int j = 0; j < N; j++) {
            bool s = true;
            auto &t2 = get<2>(photos[j]);
            for(int k : get<2>(photos[i])) {
                if(t2.find(k) == t2.end()) {
                    s = false;
                }
            }
            if(s) {
                same++;
            }
        }
        same_as[i] = same;
    }
    for(int i = 0; i < N / 10; i++) {
        cout << "Same as {" << i << "}: " << same_as[i] << endl;
    }
*/
/*    cout << "N_PICS: " << N << endl;
    cout << "N_TAGS: " << max_tag << endl;
    cout << "N_SHARED_TAGS: " << n_shared_tags << endl;
    cout << "N_VERTICALS: " << n_verticals << endl;
*/
    //photos = merge_verticals(photos);
    auto verthorz = separate_photos(photos);
    auto &verticals = verthorz.first;
    auto &horizontals = verthorz.second;
    vector<pair<int,int>> sequence;
    int max_score = 0;
    for(int i = 0; i < horizontals.size(); i++) {
        sequence.push_back({horizontals[i].first, horizontals[i].first});
    }
    for(int i = 0; i < verticals.size(); i+=2) {
        sequence.push_back({verticals[i].first, verticals[i+1].first});
    }
    
    for(int i = 0; i < sequence.size() - 1; i++) {
        max_score += calc_double_score(sequence, photos, i, i + 1);
    }


    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,sequence.size()-1); // distribution in range [1, 6]

    vector<pair<int,int>> max_sequence = sequence;

    int MAX_NEG_SCORE = 10000;
    int current_score = max_score;
    vector<pair<int,int>> current_seq = max_sequence;
    for(int i = 0; i < 1000000; i++) {
/*        if(i % 10000000 == 0) {
            cout << "DONE: " << i << endl;
        }
        */
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

    sequence = max_sequence;


    /*vector<vector<uint8_t>> pairwise_score(N, vector<uint8_t>(N));
    for(int i = 0; i < N; i++) {
        if(i % 10 == 0) {
            cout << "DONE: " << i << endl;
        }
        for(int j = i + 1; j < N; j++) {
            int sc = calc_score(photos[i], photos[j]);
            pairwise_score[i][j] = sc;
            pairwise_score[j][i] = sc;
        }
    }*/
/*
    // Find first horizontal photo
    int first = 0;
    for(int i = 0; i < photos.size(); i++) {
        if(!get<0>(photos[i])) {
            first = i;
            break;
        }
    }

    vector<bool> used(N);
    used[first] = true;
    cout << "FIRST: " << first << endl;
    // Start with photo 0
    vector<int> sequence{first};
    int tot_score = 0;
    for(int i = 1; i < N; i++) {
        // Find max horizontal 
        int cur = sequence.back();
        int max_score = -1;
        int max_index = -1;
        for(int j = 0; j < N; j++) {
            if(pairwise_score[cur][j] > max_score && !get<0>(photos[j]) && !used[j]) {
                max_index = j;
                max_score = pairwise_score[cur][j];
            }
        }

        cout << "MAXIDX: " << max_index << endl;

        if(max_index == -1) {
            break;
        }
        sequence.push_back(max_index);
        tot_score += max_score;
        used[max_index] = true;
    }
*/
    //vector<int> sequence = solveB(photos);
    cout << sequence.size() << endl;
    for(auto i : sequence) {
        if(i.first == i.second) {
            cout << i.first << "\n";
        } else {
            cout << i.first << " " << i.second << "\n";
        }
    }
    cout << endl;
}