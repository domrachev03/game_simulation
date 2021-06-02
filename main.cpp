/**
 * This is the second assignment of the ItP course
 * Implements the game and two AI which can play in it
 *
 * The work is done by Domrachev Ivan, BS20-05
 */

#include <iostream>
#include <fstream>
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <random>

using namespace std;
using namespace std::chrono_literals;

// Constant represents maximum number of milliseconds that a player is allowed to take
const int TIMEOUT = 400;

// Declaring all classes and their friend methods used in the program
class World;

class Position;

class Action;

class Figure;

class MovableFigure;

class PlayerFigure;

class Scissors;

class Rock;

class Paper;

class World;

/**
 * operator>>
 *  Overload of the reading the position from the stream
 *  Allows the convenient usage of streams
 *
 *  @param in Istream where read position from
 *  @param pos Position where to put written information
 *  @return the reference of stream in
 *  @relatedalso Position
 */
istream &operator>>(istream &in, Position &pos);

/**
 * operator<<
 *  Overload of the writing the position to the stream
 *  Allows the convenient usage of streams
 *
 *  @param out Ostream where to write the position
 *  @param pos Position for writing
 *  @return the reference of stream out
 *  @relatesalso Position
 */
ostream &operator<<(ostream &out, const Position &pos);


/**
 * operator>>
 *  Overload of the reading the action from the stream
 *  Allows the convenient usage of streams
 *
 *  @param in Istream where read action from
 *  @param action Action where to put written information
 *  @return the reference of stream in
 *  @relatesalso Action
 */
istream &operator>>(istream &in, Action &action);


/**
 * operator<<
 *  Overload of the writing the action to the stream
 *  Allows the convenient usage of streams
 *
 *  @param out Ostream where to write the action
 *  @param pos Action for writing
 *  @return the reference of stream out
 *  @relatesalso Action
 */
ostream &operator<<(ostream &out, const Action &action);

/**
 * Class Position
 *
 * Represents the position of the object in the field
 * Provides two coordinates: x and y and the convenient way to work with them
 */
class Position {
public:
    // tuple which stores two positions: x and y
    tuple<int, int> pos;

    /**
     * The default initializer of the class
     * Supposes that the indexes are 0
     */
    Position() : pos({0, 0}) {}

    /**
     * Initializer with two coordinates. Put them in the pos
     * @param x X coordinate of the position
     * @param y Y coordinate of the position
     */
    Position(int x, int y) : pos({x, y}) {}

    //ITEM 3.c: overloading operators ==, <, >, + and -
    /**
     * Overload of the comparison of the two positions.
     * Two positions are equal iff both coordinates are equal
     * @param another_pos Compared position
     * @return true if two positions are equal, false otherwise
     */
    bool operator==(const Position &another_pos) const {
        return get<0>(pos) == get<0>(another_pos.pos) &&
               get<1>(pos) == get<1>(another_pos.pos);
    }

    /**
     * Overload of less operator.
     * Used by map further on, therefore here I use coordinate-wise comparison
     * @param another_pos Compared position
     * @return true, if this position is less than given, false otherwise
     */
    bool operator<(const Position &another_pos) const {
        if (get<0>(pos) < get<0>(another_pos.pos)) {
            return true;
        }
        if (get<0>(pos) > get<0>(another_pos.pos)) {
            return false;
        }
        if (get<1>(pos) < get<1>(another_pos.pos)) {
            return true;
        }
        if (get<1>(pos) > get<1>(another_pos.pos)) {
            return false;
        }
        return false;
    }

    /**
     * Overload of bigger operator.
     * Use the same comparison as less operator
     * @param another_pos Compared position
     * @return true, if this position is bigger than given, false otherwise
     */
    bool operator>(const Position &another_pos) const {
        if (get<0>(pos) > get<0>(another_pos.pos)) {
            return true;
        }
        if (get<0>(pos) < get<0>(another_pos.pos)) {
            return false;
        }
        if (get<1>(pos) > get<1>(another_pos.pos)) {
            return true;
        }
        if (get<1>(pos) > get<1>(another_pos.pos)) {
            return false;
        }
        return false;
    }

    /**
     * Reference getter of the x coordinate
     * @return reference on x coordinate
     */
    int &x() {
        return get<0>(pos);
    }

    /**
     * Reference getter of the y coordinate
     * @return reference on y coordinate
     */
    int &y() {
        return get<1>(pos);
    }

    /**
     * Getter of the x coordinate
     * Used if we apply const reference not for changing
     * @return value of x position
     */
    int x() const {
        return get<0>(pos);
    }

    /**
     * Getter of the y coordinate
     * Used if we apply const reference not for changing
     * @return value of y position
     */
    int y() const {
        return get<1>(pos);
    }

    /**
     * Overload of sum operator
     * Sum of two positions is equal to sum two corresponding coordinates
     * @param another_pos Second term of sum
     * @return sum of two coordinates
     */
    Position operator+(Position another_pos) {
        return Position(x() + another_pos.x(), y() + another_pos.y());
    }

    /**
     * Overload of difference operator
     * Difference of two positions is equal to difference two corresponding coordinates
     * @param another_pos subtrahend of difference
     * @return difference of two coordinates
     */
    Position operator-(Position another_pos) {
        return Position(x() - another_pos.x(), y() - another_pos.y());
    }

    friend istream &operator>>(istream &in, Position &pos);

    friend ostream &operator<<(ostream &out, const Position &pos);
};

istream &operator>>(istream &in, Position &pos) {
    in >> pos.x();
    in.ignore();
    in >> pos.y();

    return in;
}

ostream &operator<<(ostream &out, const Position &pos) {
    out << '[' << pos.x() << ',' << pos.y() << ']';
    return out;
}

/**
 * Class Action
 * Represents the action made on the field.
 * Action on the field is consist of two positions:
 *      - position from - initial position
 *      - position to - position after a move
 */
class Action {
public:
    /**
     * Positions from and to
     * It was given that these two positions are public, so I didn't change it
     */
    Position from; // current row, column of the unit to be moved
    Position to; // position to where the unit must be moved

    /**
     * Default constructor of the Action
     * Calls the default constructors of the positions
     */
    Action() : from(), to() {}

    /**
     * Constructor with two positions given. Initialise from and two and the delta's of corresponding coordinates
     * @param from Position from
     * @param to Position to
     */
    Action(const Position &from, const Position &to) : from(from), to(to), x_move(to.x() - from.x()),
                                                       y_move(to.y() - from.y()) {}

    /**
     * Constructor with coordinates of from position and to position
     * Collects them to Position object and passes to the constructor with positions
     * @param x1 X coordinate of the from position
     * @param y1 Y coordinate of the from position
     * @param x2 X coordinate of the to position
     * @param y2 Y coordinate of the from position
     */
    Action(int x1, int y1, int x2, int y2) : Action({x1, y1}, {x2, y2}) {}

    /**
     * Getter of the x coordinate change
     * @return change of x coordinate
     */
    int delta_x() const {
        return x_move;
    }

    /**
     * Getter of the y coordinate change
     * @return change of y coordinate
     */
    int delta_y() const {
        return y_move;
    }

    friend istream &operator>>(istream &in, Action &action);

    friend ostream &operator<<(ostream &out, const Action &action);

private:
    /**
     * Actual change of the positions by x and y
     */
    int x_move, y_move;
};

istream &operator>>(istream &in, Action &action) {
    in >> action.from;
    in.ignore(2);
    in >> action.to;

    return in;
}

ostream &operator<<(ostream &out, const Action &action) {
    out << action.from << "->" << action.to;

    return out;
}

/**
 * Class Figure
 *  Base class of all figures
 *  Contains the basic info about the figure:
 *      - Position
 *      - Full name
 *      - Basic symbol (the first symbol in the name lowercase)
 */

class Figure {
public:
    // ITEM 3.b: Figure has several constructors and virtual destructor for inheritance
    /**
     * Default constructor of the figure
     */
    Figure() = default;

    /**
     * Virtual destructor, so children can change it
     * The main purpose of doing that is to make this type polymorphic
     */
    virtual ~Figure() = default;

    /**
     * Constructor with the name of the figure and its coordinates
     *
     * @param symbol Name of the figure
     * @param x X coordinate of the figure
     * @param y Y coordinate of the figure
     */
    Figure(string symbol, const int &x, const int &y) :
            f_name(move(symbol)), f_pos(x, y) {
        f_prime_symbol = tolower(int(f_name[0]));
    }

    /**
     * Constructor which specifies only the name of the figure
     *
     * @param symbol Name of the figure
     */
    explicit Figure(string symbol) : f_name(move(symbol)) {
        f_prime_symbol = tolower(int(f_name[0]));
    }

    /**
     * Constructor which specifies only position of the figure
     *
     * @param x X coordinate of the figure
     * @param y Y coordinate of the figure
     */
    Figure(const int &x, const int &y) : f_pos(x, y) {}

    // Default equality operator
    bool operator==(const Figure &another_figure) const = default;

    /**
     * Getter of position of the figure
     * @return Position of the figure
     */
    Position pos() const {
        return f_pos;
    }

    //ITEM 3.f: This getter is f_name is constant, as its call never changes any field of this class
    /**
     * Getter of name of the figure
     *
     * @return Name of the figure
     */
    string name() const {
        return f_name;
    }

    /**
     * Getter of prime symbol of the figure
     *
     * @return Prime symbol of the figure
     */
    char symbol() const {
        return f_prime_symbol;
    }

protected:
    Position f_pos;
    string f_name;
    char f_prime_symbol;
};

/**
 * Class MovableFigure
 *
 * Base class for all movable figures
 * Provides moving interface
 *
 * @inherit Figure
 */
class MovableFigure : virtual public Figure {
public:
    /**
     * Constructor which specified maximal moving distance of the figure
     * @param max_move_distance Maximal moving distance
     */
    MovableFigure(int max_move_distance) : max_total_move(max_move_distance) {}

    /**
     * Default constructor
     * Takes the default value of move
     */
    MovableFigure() : MovableFigure(DEFAULT_MOVE_DIST) {}

    /**
     * Getter of the maximal moving distance
     * @return Maximal moving distance
     */
    int max_move_dist() const {
        return max_total_move;
    }

    /**
     * Moving of the figure on given distance
     * @param x Move on x coordinate
     * @param y Move on y coordinate
     * @throw runtime_error, if total moving distance is bigger than allowed
     */
    void move(const int &x = 0, const int &y = 0) {
        if (abs(x) + abs(y) > max_total_move) {
            throw runtime_error("Invalid moving distance");
        }
        f_pos = f_pos + Position(x, y);
    }

protected:
    /**
     * Default limit for the moving the figures
     */
    constexpr static int DEFAULT_MOVE_DIST = 1;

private:
    int max_total_move;
};

/**
 * Class PlayerFigure
 *
 * Base class for all figures, which belong to the player
 *
 * @inherit Figure
 */
class PlayerFigure : virtual public Figure {
public:
    /**
     * Specifies the maximal amount of players
     */
    constexpr static int MAX_PLAYERS = 10;

    /**
     * Complete constructor with two symbols to construct name from and number of a player
     *
     * @param idx Number of the player
     * @param first_symbol First symbol to construct name from
     * @param second_symbol Second symbol to construct name from
     * @throw runtime_error if player index is out of allowed range
     */
    PlayerFigure(const int &idx, const char &first_symbol, const char &second_symbol) {
        if (idx <= 0 || idx > MAX_PLAYERS) {
            throw runtime_error("Invalid player index");
        }
        player_number = idx;
        f_name = createName(first_symbol, second_symbol);
        f_prime_symbol = tolower(int(f_name[0]));
    }

    /**
     * Comstructor with only one symbol for name
     * Calls complete constructor, by providing symbol as first_symbol and second_symbol
     *
     * @param idx Number of a player
     * @param symbol Symbol to construct name from
     */
    explicit PlayerFigure(const int &idx, const char &symbol) : PlayerFigure(idx, symbol, symbol) {}

    /**
     * Getter for the player index
     *
     * @return Player index
     */
    int player() const {
        return player_number;
    }

protected:
    int player_number;
private:
    /**
     * Function which builds name from two given symbols
     * The pattern is (for players from 1 to n): first_symbol, second_symbol, first_symbol+first_symbol...
     *
     * @param first_symbol First symbol to construct name
     * @param second_symbol Second symbol to construct name
     * @return Builded name
     */
    string createName(const char &first_symbol, const char &second_symbol) const {
        string name;
        for (int i = 0; i < player_number; i += 2) {
            name += (player_number % 2 == 0) ? second_symbol : first_symbol;
        }
        return name;
    }
};

/**
 * Class Scissors
 *
 * Represents one of the available unit - scissors
 *
 * @note The following classes has the same structure, so I won't specify them
 * @inherits MovableFigure, PlayerFigure
 */
class Scissors : public MovableFigure, public PlayerFigure {
public:
    /**
     * Basic symbols for constructing a name
     */
    constexpr static char LOWERCASE_SYMBOL = 's';
    constexpr static char UPPERCASE_SYMBOL = 'S';

    /**
     * Complete constructor of the Scissors
     *
     * Calls the corresponding constructors of parent classes
     *
     * @param player_idx Player number
     * @param x X coordinate of the figure
     * @param y Y coordinate of the figure
     */
    explicit Scissors(const int &player_idx, const int &x, const int &y) :
            Figure(x, y), PlayerFigure(player_idx, LOWERCASE_SYMBOL, UPPERCASE_SYMBOL),
            MovableFigure(MAX_MOVE_LENGTH) {}

    /**
     * Constructor with only info about the name
     *
     * Supposes that position of the figure is (0,0)
     *
     * @param player_idx Index of the player
     */
    explicit Scissors(int player_idx) : Scissors(player_idx, 0, 0) {}

protected:
    constexpr static int MAX_MOVE_LENGTH = 1;
};

/**
 * Class Rock
 *
 * Refer to class Scissors documentation
 * @inherits MovableFigure, PlayerFigure
 */
class Rock : public MovableFigure, public PlayerFigure {
public:
    constexpr static char LOWERCASE_SYMBOL = 'r';
    constexpr static char UPPERCASE_SYMBOL = 'R';

    explicit Rock(const int &player_idx, const int &x, const int &y) :
            Figure(x, y), PlayerFigure(player_idx, LOWERCASE_SYMBOL, UPPERCASE_SYMBOL),
            MovableFigure(MAX_MOVE_LENGTH) {}

    explicit Rock(int player_idx) : Rock(player_idx, 0, 0) {}

protected:
    constexpr static int MAX_MOVE_LENGTH = 1;
};

/**
 * Class Paper
 *
 * Refer to class Scissors documentation
 * @inherits MovableFigure, PlayerFigure
 */
class Paper : public MovableFigure, public PlayerFigure {
public:
    constexpr static char LOWERCASE_SYMBOL = 'p';
    constexpr static char UPPERCASE_SYMBOL = 'P';

    explicit Paper(const int &player_idx, const int &x, const int &y) :
            Figure(x, y), PlayerFigure(player_idx, LOWERCASE_SYMBOL, UPPERCASE_SYMBOL),
            MovableFigure(MAX_MOVE_LENGTH) {}

    explicit Paper(int player_idx) : Paper(player_idx, 0, 0) {}

protected:
    constexpr static int MAX_MOVE_LENGTH = 1;
};


/**
 * Class Flag
 *
 * Refer to class Scissors documentation
 * @inherits PlayerFigure
 */
class Flag : public PlayerFigure {
public:
    //ITEM 3.g: the flag is a unique object for each player, so it has to be non-copyable
    Flag(const Flag &another_flag) = delete;

    constexpr static char LOWERCASE_SYMBOL = 'f';
    constexpr static char UPPERCASE_SYMBOL = 'F';

    explicit Flag(const int &player_idx, const int &x, const int &y) :
            Figure(x, y), PlayerFigure(player_idx, LOWERCASE_SYMBOL, UPPERCASE_SYMBOL) {}

    explicit Flag(const int &player_idx) : Flag(player_idx, 0, 0) {}
};


/**
 * Class Mountain
 *
 * Refer to class Scissors documentation
 * @inherits Figure
 */
class Mountain : public Figure {
public:
    constexpr static char SYMBOL = 'M';

    explicit Mountain(const int &x, const int &y) : Figure(string(1, SYMBOL), x, y) {}
};

/**
 * Class World
 *
 * The main controller of the game
 * Contains info about the world, validates & performs moves etc.
 *
 */
class World {
private:
    /**
     * Checks if the given cell is visible for the given player
     * @param player_index Number of the player
     * @param pos Position of the cell
     * @return true, if it is visible, false otherwise
     */
    bool find_symbol_around(const int &player_index, const Position &pos) const {
        if (player_index < 1 || player_index > PLAYERS_AMOUNT) {
            throw runtime_error("Invalid player index");
        }
        // Checking the surrounding cells of the given one
        for (int i = pos.x() - LENGTH_OF_VISION; i <= pos.x() + LENGTH_OF_VISION; ++i) {
            for (int k = pos.y() - LENGTH_OF_VISION; k <= pos.y() + LENGTH_OF_VISION; ++k) {
                Position cur_pos(i, k);
                if (is_pos_valid(cur_pos) && field.at(cur_pos)) {
                    // If there is a player's figure in some cell, return true;
                    if (players_figures[player_index - 1].contains(field.at(cur_pos))) {
                        return true;
                    }
                }
            }
        }// If such a symbol is not found, return false
        return false;
    }

    /**
     * Set of possible units and possible player units
     */
    set<char> characters = {'f', 'm', 'p', 'r', 's', EMPTY_CELL_SYMBOL};
    set<char> player_characters = {'f', 'p', 'r', 's'};
    /**
     * Game status
     *      - If 0, games is still in process
     *      - Otherwise, contains the number of winner
     */
    int winner;
    /**
     * Map, contains the information about the result of interaction of two players
     *
     * Codes:
     *   0 - do nothing
     *   1 - second symbol moves in, first symbol dies (if not space symbol)
     *   2 - first symbol moves in, second symbol dies (if not space symbol)
     *   3 - first symbol owner wins
     *   4 - second symbol owner wins
     *   5 - illegal move
     */

    map<pair<char, char>, int> collision_codes = {
            {{'p',          'p'},          0},
            {{'p',          'r'},          2},
            {{'p',          's'},          1},
            {{'r',          'p'},          1},
            {{'r',          'r'},          0},
            {{'r',          's'},          2},
            {{'s',          'p'},          2},
            {{'s',          'r'},          1},
            {{'s',          's'},          0},
            {{'m',          'p'},          5},
            {{'m',          'r'},          5},
            {{'m',          's'},          5},
            {{'p',          'm'},          5},
            {{'r',          'm'},          5},
            {{'s',          'm'},          5},
            {{'f',          'p'},          3},
            {{'f',          'r'},          3},
            {{'f',          's'},          3},
            {{'p',          'f'},          4},
            {{'r',          'f'},          4},
            {{'s',          'f'},          4},
            {{'p',          SPACE_SYMBOL}, 2},
            {{'r',          SPACE_SYMBOL}, 2},
            {{'s',          SPACE_SYMBOL}, 2},
            {{SPACE_SYMBOL, 'p'},          1},
            {{SPACE_SYMBOL, 'r'},          1},
            {{SPACE_SYMBOL, 's'},          1},
    };

    // Vector, containing the figures of all player
    // ITEM 3.k: here is an example of STL conainer
    vector<set<shared_ptr<Figure>>> players_figures;
    /**
     * Game field itself
     *
     * Map, with figure position as key and pointer on figure
     * Unordered_map would work much faster, but I have no time to define my own hash-function
     */
    map<Position, shared_ptr<Figure>> field;

    /**
     * Validation of the position
     * @param pos Position to validate
     * @return true, if position is valid, false otherwise
     */
    bool is_pos_valid(Position pos) const {
        if (pos.x() > RIGHT_BORDER.x() || pos.y() > RIGHT_BORDER.y() ||
            pos.x() < LEFT_BORDER.x() || pos.y() < LEFT_BORDER.y()) {
            return false;
        }
        return true;
    }

    /**
     * Checker of belonging the given figure to a player
     * @param fig Pointer on a figure
     * @param player_idx Number of a player
     * @return true, if given figure belongs to a player,
     */
    bool belongs_to_player(const shared_ptr<Figure> &fig, const int &player_idx) const {
        if (!fig) return false;

        return players_figures[player_idx - 1].contains(fig);
        return players_figures[player_idx - 1].contains(fig);
    }

    /**
     * Moving figure on a given interval
     * @param fig Pointer on a figure to move
     * @param x Change of x coordinate
     * @param y Change of y coordinate
     */
    void moveFigure(shared_ptr<MovableFigure> &fig, const int &x, const int &y) {
        field[fig->pos() + Position(x, y)] = field[fig->pos()];
        field[fig->pos()] = nullptr;
        fig->move(x, y);
    }

    /**
     * Perform an interaction between two figures
     * @param moved_figure Moved figure
     * @param another_figure Figure which appeared on the way
     * @param player_number Number of a player which makes that move
     * @param action Action, which caused the interaction
     * @throw runtime_error with the description of the error
     */
    void interact(shared_ptr<Figure> moved_figure, shared_ptr<Figure> another_figure, const int &player_number,
                  const Action &action) {
        // Two symbols: moved figure's symbol and another figure's symbol
        pair<char, char> symbols = {(moved_figure) ? moved_figure->symbol() : EMPTY_CELL_SYMBOL,
                                    (another_figure) ? another_figure->symbol() : EMPTY_CELL_SYMBOL};

        if (!collision_codes.contains(symbols)) {
            throw runtime_error("Given translation does not exist!");
        }
        int action_code = collision_codes[symbols];

        auto player_movable_figure = dynamic_pointer_cast<MovableFigure>(moved_figure);
        auto another_movable_figure = dynamic_pointer_cast<MovableFigure>(another_figure);

        switch (action_code) {
            case 0:
                break;
            case 1:
                if (!another_movable_figure) {
                    throw runtime_error("Interaction error: figure cannot be moved");
                }
                field[moved_figure->pos()] = nullptr;
                if (moved_figure) {
                    for (auto &row: players_figures) {
                        for (auto it = row.begin(); it != row.end(); ++it) {
                            if (**it == *moved_figure) {
                                row.erase(it);
                                break;
                            }
                        }
                    }
                }
                break;
            case 2:
                if (!player_movable_figure) {
                    throw runtime_error("Interaction error: figure cannot be moved");
                }
                moveFigure(player_movable_figure, action.delta_x(), action.delta_y());
                if (another_figure) {
                    for (auto &row: players_figures) {
                        for (auto it = row.begin(); it != row.end(); ++it) {
                            if (**it == *another_figure) {
                                row.erase(it);
                                break;
                            }
                        }
                    }
                }
                break;
            case 3:
                throw runtime_error("Interaction error: figure considered non-player wins");
            case 4:
                if (!player_movable_figure) {
                    throw runtime_error("Interaction error: figure cannot be moved");
                }
                moveFigure(player_movable_figure, action.delta_x(), action.delta_y());
                if (another_figure) {
                    for (auto it = players_figures[1].begin(); it != players_figures[1].end(); ++it) {
                        if (**it == *another_figure) {
                            players_figures[1].erase(it);
                        }
                    }
                }
                win(player_number);
                break;
            case 5:
                win((player_number) % 2 + 1);
                break;
        }
    }

    /**
     * Default private constructor
     *
     * Initializes the game status and resizes the player figures vector
     */
    World() {
        winner = 0;
        players_figures.resize(PLAYERS_AMOUNT);
    }

    /**
     * Parces the given string field and returns the string matrix of the field
     * @param string_field String of the whole field
     * @return String matrix of the field
     */
    vector<vector<string>> parse_string_field(const string &string_field) {
        // Regex expression which takes the symbol from the cells
        regex r(R"(\s*([fFrRsSpPM\s]+)\|)");
        vector<vector<string>> matrix_field;
        // String stream for convenient string parsing line-by-line
        stringstream str(string_field);
        string cur_line;

        // Skipping the numeration and separation
        getline(str, cur_line);
        getline(str, cur_line);

        //Parsing the field
        for (int i = 0; i < FIELD_HEIGHT; ++i) {
            getline(str, cur_line);
            matrix_field.push_back(vector<string>(sregex_token_iterator(cur_line.cbegin(), cur_line.cend(), r, 1),
                                                  sregex_token_iterator()));
            // Skipping the separation of the lines
            getline(str, cur_line);
        }
        return matrix_field;
    }

public:
    /**
     * Set of constants, which contain basic info about the world
     */
    const Position LEFT_BORDER = Position(1, 1), RIGHT_BORDER = Position(15, 15);
    constexpr static int FIELD_WIDTH = 15, FIELD_HEIGHT = 15;
    constexpr static int PLAYERS_AMOUNT = 2;
    constexpr static char SPACE_SYMBOL = ' ';
    constexpr static char CELL_SEPARATE_SYMBOL = '|';
    constexpr static char EMPTY_CELL_SYMBOL = ' ';
    constexpr static char ROWS_SEPARATION_SYMBOL = '-';
    constexpr static char FOG_OF_WAR_SYMBOL = 'D';
    constexpr static int LENGTH_OF_VISION = 2;
    constexpr static int CELL_SIZE = (PLAYERS_AMOUNT + 1) / 2;

    //ITEM 3.i: using decltype to obtain the type of field, rename it and give this information outside the class
    using Field = decltype(field);

    /**
     * Constructor of the world which takes and parses the string matrix of the initial field
     *
     * @param initial_field String matrix of initial field
     * @throw runtime_error if any problem with initialisation of field occured
     */
    World(const vector<vector<string>> &initial_field) : World() {
        if (initial_field.size() != FIELD_WIDTH || initial_field[0].size() != FIELD_HEIGHT) {
            throw runtime_error("Invalid size of initial field");
        }

        for (int i = 1; i <= FIELD_WIDTH; ++i) {
            for (int k = 1; k <= FIELD_HEIGHT; ++k) {
                string cur_symbol = initial_field[i - 1][k - 1];
                char figure_type = tolower(cur_symbol[0]);

                // If symbol is not involved in the game, throw an error
                if (!characters.contains(figure_type)) {
                    throw runtime_error("Unexpected symbol in initial field");
                }

                // Calcualting player number from its name. Makes no sense, if doesn't belongs to a player
                int player = (cur_symbol.size() + 1) / 2 + (cur_symbol[0] != figure_type);

                shared_ptr<Figure> figure;

                switch (figure_type) {
                    case 's':
                        figure = make_shared<Scissors>(player, k, i);
                        break;
                    case 'r':
                        figure = make_shared<Rock>(player, k, i);
                        break;
                    case 'p':
                        figure = make_shared<Paper>(player, k, i);
                        break;
                    case 'f':
                        figure = make_shared<Flag>(player, k, i);
                        break;
                    case 'm':
                        figure = make_shared<Mountain>(k, i);
                        break;
                    case EMPTY_CELL_SYMBOL:
                        figure = nullptr;
                        break;
                    default:
                        throw runtime_error("Unexpected symbol in initial field (some problems in characters list)");
                }

                field[{k, i}] = figure;

                // If the symbol is belongs to any player, add it to the corresponging vector
                if (player_characters.contains(figure_type)) {
                    players_figures[player - 1].insert(figure);
                }
            }
        }
    }

    /**
     * Constructor of the world from string initial field
     *
     * Transforms the string to the string matrix and passes it to the corresponding constructor
     *
     * @param initial_field String of the initial field
     * @throw errors from another constructor
     */
    World(const string &initial_field) : World(parse_string_field(initial_field)) {}

    /**
     * Getter of the game status
     *      - if game is in process, returns 0
     *      - Otherwise, returns the number of a player (bigger than 0)
     * @return Game status
     */
    int game_status() const {
        return winner;
    }

    /**
     * Overload of getter of the game field
     *
     * Transforms the whole field (which is the map) to a pointer matrix and return it
     *
     * @return Pointer matrix field
     */
    vector<vector<shared_ptr<Figure>>> get_field() const {
        vector<vector<shared_ptr<Figure>>> field_matrix(FIELD_WIDTH, vector<shared_ptr<Figure>>(FIELD_HEIGHT));
        for (int i = 1; i <= FIELD_WIDTH; ++i) {
            for (int k = 1; k <= FIELD_HEIGHT; ++k) {
                field_matrix[i - 1][k - 1] = field.at({i, k});
            }
        }
        return field_matrix;
    }
    /*
     * ITEM 3.a: I rewrite the functions, which return the info about the field
     * It get a vector of player numbers. Function will show the visible area for this players
     * If there is no arguments, show the whole map
     */
    /**
     * Getter of the game field with figures, visible only for given players
     *
     * @param players_idx vector of indices of players for which this field is returned
     * @return the field with fog of war
     */
    vector<vector<shared_ptr<Figure>>> get_field(const vector<int> &players_idx) const {
        for (const auto &player: players_idx) {
            if (player < 1 || player > PLAYERS_AMOUNT) {
                throw runtime_error("Invalid player index");
            }
        }
        vector<vector<shared_ptr<Figure>>> field_matrix(FIELD_WIDTH, vector<shared_ptr<Figure>>(FIELD_HEIGHT));
        for (int i = 1; i <= FIELD_WIDTH; ++i) {
            for (int k = 1; k <= FIELD_HEIGHT; ++k) {
                bool is_visible = false;
                for (const auto &player: players_idx) {
                    if (find_symbol_around(player, {i, k})) {
                        is_visible = true;
                        break;
                    }
                }
                if (is_visible) {
                    field_matrix[i - 1][k - 1] = field.at({i, k});
                } else {
                    field_matrix[i - 1][k - 1] = nullptr;
                }
            }
        }
        return field_matrix;
    }

    /**
     * Overload of a function, which builds the string of the whole field and returns it
     *
     * @return String of the field
     */
    string get_field_string() const {
        // The maximal size of the number of the field
        int number_size = 0;

        //Calculating the maximal size of the number
        int temp_width = FIELD_WIDTH, temp_height = FIELD_HEIGHT;
        while (temp_width != 0 || temp_height != 0) {
            temp_width /= 10;
            temp_height /= 10;
            number_size++;
        }

        // Size of the cell of the field
        int cell_size = max(CELL_SIZE, number_size);
        // Total size of the row
        int row_size = number_size + (cell_size + 1) * FIELD_WIDTH + 1;

        // Separation of the rows between each other
        string separation;
        if (ROWS_SEPARATION_SYMBOL == ' ') {
            separation = "";
        }
        separation = string(row_size, ROWS_SEPARATION_SYMBOL);

        // The string field
        string field_str(number_size, 'X');
        // Current number of the column/row
        string number;
        //Amount of empty space needed to fill the cell
        int empty_space_amount;

        // Initialising the first row with column's numbers
        for (int i = 1; i <= FIELD_WIDTH; ++i) {
            number = to_string(i);
            empty_space_amount = cell_size - number.size();
            field_str += CELL_SEPARATE_SYMBOL + string(empty_space_amount, SPACE_SYMBOL) + to_string(i);
        }

        field_str = field_str + CELL_SEPARATE_SYMBOL + '\n' + separation + '\n';

        // Adding the rest field
        for (int i = 1; i <= FIELD_HEIGHT; ++i) {
            // Adding the row number
            number = to_string(i);
            empty_space_amount = cell_size - number.size();
            field_str += string(empty_space_amount, SPACE_SYMBOL) + number;

            // Adding the rest field
            for (int k = 1; k <= FIELD_WIDTH; ++k) {
                string name;
                name = (field.at({k, i})) ? field.at({k, i})->name() : "";
                empty_space_amount = cell_size - name.size();
                field_str +=
                        CELL_SEPARATE_SYMBOL + string(empty_space_amount, EMPTY_CELL_SYMBOL) + name;
            }
            field_str = field_str + CELL_SEPARATE_SYMBOL + '\n' + separation + '\n';
        }

        return field_str;
    }

    /**
     * Gets field string for given players (considers fog of war)
     * @param players_idx Vector of indices of players, for which the area is visible
     * @return the string with a field with fog of war
     */
    string get_field_string(const vector<int> &players_idx) const {
        // The maximal size of the number of the field
        int number_size = 0;

        //Calculating the maximal size of the number
        int temp_width = FIELD_WIDTH, temp_height = FIELD_HEIGHT;
        while (temp_width != 0 || temp_height != 0) {
            temp_width /= 10;
            temp_height /= 10;
            number_size++;
        }

        // Size of the cell of the field
        int cell_size = max(CELL_SIZE, number_size);
        // Total size of the row
        int row_size = number_size + (cell_size + 1) * FIELD_WIDTH + 1;

        // Separation of the rows between each other
        string separation;
        if (ROWS_SEPARATION_SYMBOL == ' ') {
            separation = "";
        }
        separation = string(row_size, ROWS_SEPARATION_SYMBOL);

        // The string field
        string field_str(number_size, 'X');
        // Current number of the column/row
        string number;
        //Amount of empty space needed to fill the cell
        int empty_space_amount;

        // Initialising the first row with column's numbers
        for (int i = 1; i <= FIELD_WIDTH; ++i) {
            number = to_string(i);
            empty_space_amount = cell_size - number.size();
            field_str += CELL_SEPARATE_SYMBOL + string(empty_space_amount, SPACE_SYMBOL) + to_string(i);
        }

        field_str = field_str + CELL_SEPARATE_SYMBOL + '\n' + separation + '\n';

        // Adding the rest field
        for (int i = 1; i <= FIELD_HEIGHT; ++i) {
            // Adding the row number
            number = to_string(i);
            empty_space_amount = cell_size - number.size();
            field_str += string(empty_space_amount, SPACE_SYMBOL) + number;

            // Adding the rest field
            for (int k = 1; k <= FIELD_WIDTH; ++k) {
                string name;
                bool is_visible = false;
                Position pos(k, i);
                for (const auto &player: players_idx) {
                    if (find_symbol_around(player, pos)) {
                        is_visible = true;
                        break;
                    }
                }
                if (!is_visible) {
                    name = string(cell_size, FOG_OF_WAR_SYMBOL);
                } else {
                    name = (field.at({k, i})) ? field.at({k, i})->name() : "";
                }
                empty_space_amount = cell_size - name.size();
                field_str +=
                        CELL_SEPARATE_SYMBOL + string(empty_space_amount, EMPTY_CELL_SYMBOL) + name;
            }
            field_str = field_str + CELL_SEPARATE_SYMBOL + '\n' + separation + '\n';
        }

        return field_str;
    }

    /**
     * Getter of all figures of given player
     *
     * @param Player Number of a player
     * @return Vector of pointers on player figures
     * @throw runtime_error in case of invalid index
     */
    set<shared_ptr<Figure>> getPlayerFigures(int player) const {
        if (player < 1 || player > PLAYERS_AMOUNT) {
            throw runtime_error("Player with number " + to_string(player) + " does not exist");
        }
        return players_figures[player - 1];
    }

    /**
     * Perform the win process of a given player
     * @param player_idx Number of a winner
     */
    void win(const int &player_idx) {
        winner = player_idx;
    }

    /**
     * Takes the actions and performs them
     * @param actions Vector of actions made by players
     * @throw runtime_error with the description of the problem
     */
    void apply_moves(const vector<Action> &actions) {
        if (actions.size() != 2) { // equal to if(actions.size() != PLAYER_AMOUNT, till extension
            throw runtime_error("Invalid amount of actions (more than two players does not supported yet)");
        }
        if (winner != 0) {
            return;
        }
        /**
         * Checking of the validness of the poses in move
         * If the position of any move is invalid, another player wins (move is illegal)
         */
        for (int i = 0; i < 2; ++i) {
            if (!is_pos_valid(actions[i].from) || !is_pos_valid(actions[i].to)) {
                switch (i) {
                    case 0:
                        win(2);
                        return;
                    case 1:
                        win(1);
                        return;
                }
            }
        }

        //ITEM 3.d: dynamic upcasting of a pointer for using this object as movable figure

        // Casting the figures to MovableFigure to move them later
        auto player1_figure = dynamic_pointer_cast<MovableFigure>(field[actions[0].from]);
        auto player2_figure = dynamic_pointer_cast<MovableFigure>(field[actions[1].from]);

        // If any of the figures chosed is invalid, another player wins (move is illegal)
        if (!player1_figure || !player2_figure) {
            if (!player1_figure) {
                win(2);
                return;
            }
            if (!player2_figure) {
                win(2);
                return;
            }
        }

        //If the figure does not belongs to a player, another player wins (move is illegal)
        if (!belongs_to_player(player1_figure, 1)) {
            win(2);
            return;
        } else if (!belongs_to_player(player2_figure, 2)) {
            win(1);
            return;
        }

        // Pointers to figures, which are on the way of movement
        shared_ptr<Figure> player1_moving_figure = field[actions[0].to];
        shared_ptr<Figure> player2_moving_figure = field[actions[1].to];

        // If any player decides to move at his own figure or don't change the position, another player wins
        // (move is illegal)
        if (belongs_to_player(player1_moving_figure, 1) ||
            (player1_moving_figure && *player1_figure == *player1_moving_figure)) {
            win(2);
            return;
        } else if (belongs_to_player(player2_moving_figure, 2) ||
                   (player2_moving_figure && *player2_figure == *player2_moving_figure)) {
            win(1);
            return;
        }
            // If everything is fine, perform the moves
        else {
            // Hardcoding of bouncing back. Didn't find a way to analyse it in a common way. In this case, do nothing
            if (actions[0].to == actions[1].to && player1_figure->symbol() == player2_figure->symbol()) {
                return;
            }
            // Avoiding the case of killing before player moves away his figure
            if (actions[0].to == player2_figure->pos()) {
                interact(field[actions[1].from], field[actions[1].to], 2, actions[1]);
                interact(field[actions[0].from], field[actions[0].to], 1, actions[0]);
            } else {
                interact(field[actions[0].from], field[actions[0].to], 1, actions[0]);
                interact(field[actions[1].from], field[actions[1].to], 2, actions[1]);
            }
        }
    }
};

/**
 * First player AI
 *
 * Performs pseudorandom moves
 *
 * @param const reference on world World controller
 * @return Action made
 */
template<int player_number>
Action AI(const World &world) {
    Action newAction;

    // Creating vector of all player figures and inserting elements of set here
    vector<shared_ptr<Figure>> figures;
    for (const auto &element: world.getPlayerFigures(player_number)) {
        figures.push_back(element);
    }

    //Loop of choosing the move
    bool actionIsReady = false;
    while (!actionIsReady) {

        //Choosing the random element; casting it to a movable figure pointer
        int random_element_pos = rand() % figures.size();
        auto movable_random_element = dynamic_pointer_cast<MovableFigure>(figures[random_element_pos]);
        if (movable_random_element) {
            auto element_position = movable_random_element->pos();
            auto current_field = world.get_field({player_number});

            // Collecting the positions, available for the move
            // i - delta of x coordinate, k - delta of y coordinate, |i|+|k| <= max_move_dist
            vector<Position> available_positions;
            for (int i = -movable_random_element->max_move_dist(); i <= movable_random_element->max_move_dist(); ++i) {
                for (int k = abs(i) - movable_random_element->max_move_dist();
                     k <= movable_random_element->max_move_dist() - abs(i); ++k) {
                    // Obtaining the position after move, check it and add to the vector, if it is valid
                    int xCoord = movable_random_element->pos().x() + i, yCoord = movable_random_element->pos().y() + k;
                    if (xCoord < 1 || xCoord > world.FIELD_WIDTH || yCoord < 1 || yCoord > world.FIELD_HEIGHT) {
                        continue;
                    }
                    auto player_figure = dynamic_pointer_cast<PlayerFigure>(current_field[xCoord - 1][yCoord - 1]);
                    if (!current_field[xCoord - 1][yCoord - 1] ||
                        (player_figure && player_figure->player() != player_number)) {
                        available_positions.push_back({xCoord, yCoord});
                    }
                }
            }
            // If there is some acceptable position, choose random from them
            if (available_positions.size() != 0) {
                actionIsReady = true;
                int random_action = rand() % available_positions.size();
                newAction = Action(element_position, available_positions[random_action]);
            }
        }
    }

    return newAction;
}

/**
 * Comparator of the figures to sort them by distance to the flag
 * @param p1 First pointer at figure
 * @param p2 Second pointer at figure
 * @return true, if p1 < p2, false otherwise
 */
bool comp(shared_ptr<Figure> &p1, shared_ptr<Figure> &p2) {
    return (p1->pos().x() + p1->pos().y()) < (p2->pos().x() + p2->pos().y());
}

/**
 * Overload of random AI with the second player AI
 *
 * Takes the closest unit and moves it closer to a flag
 *
 * @param const reference on world World controller
 * @return Action made
 */

//ITEM 3.h: explicit specialization of AI of second player instead of pseudorandom

template<>
Action AI<2>(const World &world) {
    Action newAction;
    bool actionIsReady = false;

    // Creating vector of all player figures and inserting elements of set here
    vector<shared_ptr<Figure>> figures;
    for (const auto &element: world.getPlayerFigures(2)) {
        figures.push_back(element);
    }

    // Sorting the vector by the distance from (1;1) position
    //ITEM 3.j: using predicate function (comparator) to sort the figures
    sort(figures.begin(), figures.end(), comp);

    // Iterating through sorted list and try to make a move
    for (const auto &figure: figures) {
        auto movable_element = dynamic_pointer_cast<MovableFigure>(figure);
        if (movable_element) {

            auto element_position = movable_element->pos();
            auto current_field = world.get_field({2});

            // Looking for positions with certain rules
            vector<Position> good_positions;
            for (int i = -movable_element->max_move_dist(); i <= movable_element->max_move_dist(); ++i) {
                for (int k = abs(i) - movable_element->max_move_dist();
                     k <= movable_element->max_move_dist() - abs(i); ++k) {
                    int xCoord = movable_element->pos().x() + i, yCoord = movable_element->pos().y() + k;
                    if (xCoord < 1 || xCoord > world.FIELD_WIDTH || yCoord < 1 || yCoord > world.FIELD_HEIGHT) {
                        continue;
                    }
                    auto player_figure = dynamic_pointer_cast<PlayerFigure>(current_field[xCoord - 1][yCoord - 1]);

                    /*
                     * Rules:
                     *      1. If player can capture the flag, he does it
                     *      2. Otherwise, try to make move to the empty cell or to eat some opponent's figure
                     */

                    if (!current_field[xCoord - 1][yCoord - 1] || (player_figure && player_figure->player() != 2)) {

                        if (player_figure && player_figure->symbol() == 'f' && player_figure->player() != 2) {
                            newAction = Action(element_position, {xCoord, yCoord});
                            return newAction;
                        }

                        if ((!player_figure && i <= 0 && k <= 0) || (player_figure &&
                                                                     (player_figure->symbol() == 'p' &&
                                                                      movable_element->symbol() == 's' ||
                                                                      player_figure->symbol() == 's' &&
                                                                      movable_element->symbol() == 'r' ||
                                                                      player_figure->symbol() == 'r' &&
                                                                      movable_element->symbol() == 'p'))) {
                            good_positions.push_back({xCoord, yCoord});
                        }
                    }
                }
            }
            // If there is any good actions, choose random from them
            if (good_positions.size() != 0) {
                actionIsReady = true;
                int random_action = rand() % good_positions.size();
                newAction = Action(element_position, good_positions[random_action]);
            }
        }
    }
    return newAction;
}

/**
 * The return is a pair: action and a boolean whether a timeout happened
 */
tuple<Action, bool> waitPlayer(Action (*f)(const World &), const World &world) {
    auto start = chrono::high_resolution_clock::now();
    // TODO Important. Below, action should be the result of calling the function passed as parameter
    Action action = f(world);
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> elapsed = end - start;
    cout << "Waited " << elapsed.count() << " ms\n";
    if (elapsed.count() > TIMEOUT) // if time > 0.3 s
        return {action, true}; // player failed to answer in less than 400 ms
    else return {action, false};
}

/**
 * Loader of a field from the file input.txt
 * @return string of the field and actions at the file
 */
tuple<string, vector<Action>> get_field() {
    ifstream in("input.txt");

    string field;
    vector<Action> actions;
    string tmp;
    Action tmp_action;
    for (int i = 0; i <= World::FIELD_HEIGHT * 2; ++i) {
        getline(in, tmp);
        field += tmp + '\n';
    }
    while (getline(in, tmp)) {
        stringstream str;
        str >> tmp_action;
        actions.push_back(tmp_action);
    }
    return {field, actions};
}

/**
 * Saving the progress and all moves in the game
 * @param field String of the field
 * @param actions Vector of all actions made
 */

void load_history(const string &field, const vector<Action> &actions) {
    ofstream out("output.txt");
    out << field << '\n';
    for (const auto &action: actions) {
        out << action << '\n';
    }
}

/*
    The example of the field:
XX| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|
------------------------------------------------
 1| f| r| r| r| r| r|  |  |  |  |  |  |  |  |  |
------------------------------------------------
 2|  | p| p| p| p| p|  |  |  |  |  |  |  |  |  |
------------------------------------------------
 3|  | s| s| s| s| s|  |  |  |  | M|  | M|  |  |
------------------------------------------------
 4|  | r| r| r| r| r|  |  |  |  |  |  |  |  |  |
------------------------------------------------
 5|  | p| p| p| p| p|  |  |  | M|  |  |  | M|  |
------------------------------------------------
 6|  | s| s| s| s| s|  |  |  |  | M| M| M|  |  |
------------------------------------------------
 7|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
------------------------------------------------
 8|  |  |  |  |  |  | M| M| M|  |  |  |  |  |  |
------------------------------------------------
 9|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
------------------------------------------------
10|  |  |  |  |  |  |  |  |  | S| S| S| S| S|  |
------------------------------------------------
11|  |  | M|  | M|  | M|  |  | P| P| P| P| P|  |
------------------------------------------------
12|  |  | M|  | M|  | M|  |  | R| R| R| R| R|  |
------------------------------------------------
13|  |  | M|  | M| M| M|  |  | S| S| S| S| S|  |
------------------------------------------------
14|  |  |  |  |  |  |  |  |  | P| P| P| P| P|  |
------------------------------------------------
15|  |  |  |  |  |  |  |  |  | R| R| R| R| R| F|
------------------------------------------------

The example of the moves:
[6,4]->[7,4]
[10,10]->[9,10]
[6,2]->[7,2]
[9,10]->[9,9]
[2,6]->[1,6]
[9,9]->[8,9]
[6,3]->[7,3]
[8,9]->[7,9]
[7,4]->[8,4]
*/

int main() {
    srand(time(0));
    auto[str_field, actions] = get_field();
    World world(str_field);
    vector<Action> logs;

    bool endGame = false;
    while (!endGame) {
        system("@cls||clear");
        auto start = chrono::high_resolution_clock::now();
        auto[action0, timeout0] = waitPlayer(AI<1>, world);
        auto[action1, timeout1] = waitPlayer(AI<2>, world);
        if (timeout0 || timeout1) {
            endGame = true;
            cout << "Player with number ";
            if (timeout0) {
                cout << 2;
            } else {
                cout << 1;
            }
            cout << " won!\n";
            break;
        } else {
            world.apply_moves({action0, action1});
            if (world.game_status() != 0) {
                endGame = true;
                cout << "Player with number " + to_string(world.game_status()) + " won!\n";
            }
            cout << action0 << '\n' << action1 << '\n' << world.get_field_string({1, 2})
                 << "-------------------------------------------\n";
        }
        auto end = chrono::high_resolution_clock::now();
        logs.push_back(action0);
        logs.push_back(action1);
        chrono::duration<double, milli> elapsed = end - start;
        this_thread::sleep_for(chrono::milliseconds(1000 - int(elapsed.count())));
    }
    ofstream out("game_logs.txt");
    out << str_field;
    for (const auto &move: logs) {
        out << move << '\n';
    }
    out << world.get_field_string();
    return 0;
}
