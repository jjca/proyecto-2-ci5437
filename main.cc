// Game of Othello -- Example of main
// Universidad Simon Bolivar, 2012.
// Author: Blai Bonet
// Last Revision: 1/11/16
// Modified by: 

#include <iostream>
#include <limits>
#include "othello_cut.h" // won't work correctly until .h is fixed!
#include "utils.h"

#include <unordered_map>

using namespace std;

#include <limits.h>

unsigned expanded = 0;
unsigned generated = 0;
int tt_threshold = 32; // threshold to save entries in TT

// Transposition table (it is not necessary to implement TT)
struct stored_info_t {
    int value_;
    int type_;
    enum { EXACT, LOWER, UPPER };
    stored_info_t(int value = -100, int type = LOWER) : value_(value), type_(type) { }
};

struct hash_function_t {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public unordered_map<state_t, stored_info_t, hash_function_t> {
};

hash_table_t TTable[2];

//int maxmin(state_t state, int depth, bool use_tt);
//int minmax(state_t state, int depth, bool use_tt = false);
//int maxmin(state_t state, int depth, bool use_tt = false);
/*
Recibe un estado, una profundidad y un color. El atributo use_tt no se está usando
*/
int negamax(state_t state, int depth, int color, bool use_tt = false){
    // Suma 1 a los nodos generados
    generated++;
    // Si el árbol alcanzó la profundidad máxima o es terminal, retorna el valor.
    if (depth==INT_MIN || state.terminal()) {
        return color * state.value();
    };
    // Inicializa alpha y la variable booleana para el jugador
    int alpha = INT_MIN;
    bool player = color == 1 ? true : false;
    // Arreglo de los estados hijos del estado actual para el jugador actual
    vector<int> childs = state.possible_moves(player);
    // Mientras hayan hijos por revisar:
    if (!childs.empty()) { 
        for (int child_position : childs) {
            expanded++;
            // El estado es generado con la función move
            state_t state_movement = state.move(player,child_position);
            // Se calcula Alpha
            alpha = max(alpha,-negamax(state_movement,depth-1,-color,use_tt=false));
        };
    }
    // si no hay, ubico el alpha 
    else {
        alpha = max(alpha,-negamax(state,depth-1,-color,use_tt=false));        
    }
    return alpha;
}

int negamax(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false) {
    generated++;
    
    // Si el árbol alcanzó la profundidad máxima o es terminal, retorna el valor.
    if (depth==INT_MIN || state.terminal()) {
        return color * state.value();
    };
    
    // Inicializa alpha y la variable booleana para el jugador
    int score = INT_MIN;
    bool player = color == 1 ? true : false;
    
    // Arreglo de los estados hijos del estado actual para el jugador actual
    vector<int> childs = state.possible_moves(player);
    
    // Mientras hayan hijos por revisar:
    if (!childs.empty()) { 
        for (int child_position : childs) {
            expanded++;
            
            // El estado es generado con la función move
            state_t state_movement = state.move(player,child_position);
            
            // Se calcula Alpha
            int value = -negamax(state_movement,depth-1,-beta,-alpha,-color,use_tt=false);
            score = max(score,value);
            alpha = max(alpha,value);
            if (alpha >= beta) {
                break;
            }
        };
    }
    
    // si no hay hijos, ubico el alpha 
    else {
        int value = -negamax(state,depth-1,-beta,-alpha,-color,use_tt=false);
        score = max(score,value);
    }
    return score;
}

bool test(state_t state, int depth, int color, int score, bool condition) {
    if (depth == INT_MIN || state.terminal()) {
        if (condition) {
            return state.value() > score ? true : false;
        }
        return state.value() >= score ? true : false;
    }

    bool player = color == 1 ? true : false;
    
    // Arreglo de los estados hijos del estado actual para el jugador actual
    vector<int> childs = state.possible_moves(player);
    
    // Mientras hayan hijos por revisar:
    if (!childs.empty()) { 
        for (int child_position : childs) {
            expanded++;
            // El estado es generado con la función move
            state_t state_movement = state.move(player,child_position);

            if (player && test(state_movement,depth-1,-color,score,condition)) {
                return true;
            }

            if (!player && !test(state_movement,depth-1,-color,score,condition)) {
                return false;
            }            
        };
    }
    
    // si no hay hijos, ubico el score
    else {
        if (player && test(state,depth-1,-color,score,condition)) {
            return true;
        }
        if (!player && !test(state,depth-1,-color,score,condition)) {
            return false;
        }
    }

    return player ? false : true;
}

int scout(state_t state, int depth, int color, bool use_tt = false) {
    generated++;
    bool first_child = true;
    // Si el árbol alcanzó la profundidad máxima o es terminal, retorna el valor.
    if (depth == INT_MIN || state.terminal()) {
        return state.value();
    };

    int score = 0;
    // True is White
    // False is Black
    bool player = color == 1 ? true : false;
    
    // Arreglo de los estados hijos del estado actual para el jugador actual
    vector<int> childs = state.possible_moves(player);
    
    // Mientras hayan hijos por revisar:
    if (!childs.empty()) { 
        for (int child_position : childs) {
            expanded++;
            // El estado es generado con la función move
            state_t state_movement = state.move(player,child_position);
            //int value = scout(state_movement,depth-1,-color,false);
            if (first_child) {
                first_child = false;
                score = scout(state_movement,depth-1,-color,false);
            }
            else {
                if (player && test(state_movement,depth-1,-color,score,true)) {
                    score = scout(state_movement,depth-1,-color,false);
                }
                if (!player && !test(state_movement,depth-1,-color,score,false)) {
                    score = scout(state_movement,depth-1,-color,false);
                }
            }            
        };
    }
    
    // si no hay hijos, ubico el score
    else {
        score = scout(state,depth-1,-color,false);
    }
    return score;

}


int negascout(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false) {
    generated++;
    bool first_child = true;
    // Si el árbol alcanzó la profundidad máxima o es terminal, retorna el valor.
    if (depth == INT_MIN || state.terminal()) {
        return color * state.value();
    };

    int score = 0;
    // True is White
    // False is Black
    bool player = color == 1 ? true : false;
    
    // Arreglo de los estados hijos del estado actual para el jugador actual
    vector<int> childs = state.possible_moves(player);
    // Mientras hayan hijos por revisar:
    if (!childs.empty()) { 
        for (int child_position : childs) {
            expanded++;
            // El estado es generado con la función move
            state_t state_movement = state.move(player,child_position);
            //int value = scout(state_movement,depth-1,-color,false);
            if (first_child) {
                first_child = false;
                score = -negascout(state_movement,depth-1,-beta,-alpha,-color,false);
            }
            else {
                score = -negascout(state_movement,depth-1,-alpha-1,-alpha,-color,false);
                if (alpha < score && score < beta) {
                    score = -negascout(state_movement,depth-1,-beta,-score,-color,false);
                }
            } 
            alpha = max(alpha,score);
            if (alpha >= beta) {
                break;
            };
        };
    }
    // si no hay hijos, ubico el score
    else {
        alpha = -negascout(state,depth-1,-beta,-alpha,-color,false);
    }
    

    return alpha;
}



int main(int argc, const char **argv) {
    state_t pv[128];
    int npv = 0;
    for( int i = 0; PV[i] != -1; ++i ) ++npv;

    int algorithm = 0;
    if( argc > 1 ) algorithm = atoi(argv[1]);
    bool use_tt = argc > 2;
    // Extract principal variation of the game
    state_t state;
    cout << "Extracting principal variation (PV) with " << npv << " plays ... " << flush;
    for( int i = 0; PV[i] != -1; ++i ) {
        bool player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        pv[npv - i] = state;
        state = state.move(player, pos);
    }
    pv[0] = state;
    cout << "done!" << endl;
#if 0
    // print principal variation
    for( int i = 0; i <= npv; ++i )
        cout << pv[npv - i];
#endif

    // Print name of algorithm
    cout << "Algorithm: ";
    if( algorithm == 1 )
        cout << "Negamax (minmax version)";
    else if( algorithm == 2 )
        cout << "Negamax (alpha-beta version)";
    else if( algorithm == 3 )
        cout << "Scout";
    else if( algorithm == 4 )
        cout << "Negascout";
    cout << (use_tt ? " w/ transposition table" : "") << endl;

    // Run algorithm along PV (bacwards)
    cout << "Moving along PV:" << endl;
    for( int i = 0; i <= npv; ++i ) {
        //cout << pv[i];
        int value = 0;
        TTable[0].clear();
        TTable[1].clear();
        float start_time = Utils::read_time_in_seconds();
        expanded = 0;
        generated = 0;
        int color = i % 2 == 1 ? 1 : -1;

        try {
            if( algorithm == 1 ) {
                value = negamax(pv[i], 0, color, use_tt);
            } else if( algorithm == 2 ) {
                value = negamax(pv[i], 0, -200, 200, color, use_tt);
            } else if( algorithm == 3 ) {
                value = scout(pv[i], 0, color, use_tt);
            } else if( algorithm == 4 ) {
                value = negascout(pv[i], 0, -200, 200, color, use_tt);
            }
        } catch( const bad_alloc &e ) {
            cout << "size TT[0]: size=" << TTable[0].size() << ", #buckets=" << TTable[0].bucket_count() << endl;
            cout << "size TT[1]: size=" << TTable[1].size() << ", #buckets=" << TTable[1].bucket_count() << endl;
            use_tt = false;
        }

        float elapsed_time = Utils::read_time_in_seconds() - start_time;

        cout << npv + 1 - i << ". " << (color == 1 ? "Black" : "White") << " moves: "
             << "value=" << color * value
             << ", #expanded=" << expanded
             << ", #generated=" << generated
             << ", seconds=" << elapsed_time
             << ", #generated/second=" << generated/elapsed_time
             << endl;
    }

    return 0;
}

