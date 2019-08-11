//
// Created by Bobby on 10/08/2019.
//

#ifndef STRINGTOSTRING_MAIN_H
#define STRINGTOSTRING_MAIN_H

#define BUFFER_SIZE 100
#define _ever ;;
#define _and &&
#define _or ||
#define ref(arg) &arg
#define deref(arg) *arg
#define sc struct
#define en enum


sc Bin {
    void **list;
    int capacity, size;

    // Functions //
    void (*__trash_this_ptr)(sc Bin *self, void *ptr);

    void (*__take_out_the_trash)(sc Bin **self);

    void (*_get_bigger_bin)(sc Bin *self);
};

en Op {
    NON     // 0
    , INS   // 1
    , RMV   // 2
    , REP   // 3
};

sc Node {
    int i, j, stp;
    en Op op;
    sc Node *p;

    // Functions //
    int (*__equals)(sc Node *self, sc Node *node);
    char *(*__to_string)(sc Node *self, char *str, sc Bin *bin);
};

sc Grid {
    long w, h;
    sc Node **list;

    // Functions //
    void (*__add_node)(sc Grid *self, sc Node *node);

    sc Node *(*__get_node)(sc Grid *self, int i, int j);
};

sc Writer {
    FILE *file;

    // Functions //
    void (*__write_line)(sc Writer *self, char *line);
    void (*__close)(sc Writer *self);
};

sc Reader {
    FILE *file;

    // Functions //
    char *(*__next_line)(sc Reader *self, sc Bin *bin);

    void (*__close)(sc Reader *self);
};

sc Package {
    char *str1, *str2;
    sc Grid *grid;
    sc Bin *bin;
    sc Writer *writer;
    sc Reader *reader;
};


// Function Prototype //
/**<--------------------               Bin              -------------------->**/
/**
 * Create a new Bin
 * @return the Bin
 */
sc Bin *new_bin(void);

/**
 * Put pointer into bin.
 * @param bin
 * @param ptr
 */
void add_to_bin(sc Bin *self, void *ptr);

/**
 * Increase the size of the bin
 * @param bin
 */
void increase_bin_size(sc Bin *self);

/**
 * Free all dynamically allocated memories collected in the bin
 * @param bin
 */
void remove_all(sc Bin **self);
/**<------------------------------------------------------------------------>**/

/**<--------------------              Grid              -------------------->**/
/**
 * Create new grid of width x height, and put all alloc'd memory into bin.
 * @param width
 * @param height
 * @param bin
 * @return
 */
sc Grid *new_grid(long width, long height, sc Bin *bin);

/**
 * Add node to grid
 * @param self
 * @param node
 */
void add_node(sc Grid *self, sc Node *node);

/**
 * Get node at index i, j
 * @param self
 * @param i
 * @param j
 * @return
 */
sc Node *get_node(sc Grid *self, int i, int j);
/**<------------------------------------------------------------------------>**/

/**<--------------------              Node              -------------------->**/
/**
 * Create new node, and put all alloc'd memory into bin.
 * @param steps
 * @param op
 * @param i
 * @param j
 * @param parent
 * @param bin
 * @return
 */
sc Node *new_node(int steps, en Op op, int i, int j,
                  sc Node **parent, sc Bin *bin);

/**
 * Return the node with the smallest amount of steps.
 * @param n1
 * @param n2
 * @param n3
 * @return
 */
sc Node *min_node(sc Node *n1, sc Node *n2, sc Node *n3);

/**
 * Compare two nodes.
 * @param self
 * @param node
 * @return
 */
int node_equals(sc Node *self, sc Node *node);

/**
 * To string method for node.
 * @param self
 * @param str
 * @param bin
 * @return
 */
char *node_to_string(sc Node *self, char *str, sc Bin *bin);
/**<------------------------------------------------------------------------>**/

/**<--------------------             Writer             -------------------->**/
/**
 * Create new writer, must manually fclose() after done.
 * @param file
 * @return
 */
sc Writer *new_writer(char *file, sc Bin *bin);

/**
 * Write line to file.
 * @param self
 * @param line
 */
void write_line(sc Writer *self, char *line);

/**
 * Closes file
 * @param self
 */
void close_writer(sc Writer *self);
/**<------------------------------------------------------------------------>**/

/**<--------------------             Reader             -------------------->**/
/**
 * Create new reader, must manually fclose() after done.
 * @param file
 * @param bin
 * @return
 */
sc Reader *new_reader(char *file, sc Bin *bin);

/**
 * Read a single line from file opened in Reader.
 * @param self
 * @param bin
 * @return
 */
char *read_line(sc Reader *self, sc Bin *bin);

/**
 * Closes file
 * @param reader
 */
void close_reader(sc Reader *reader);
/**<------------------------------------------------------------------------>**/

/**<--------------------            Package             -------------------->**/
/**
 * Create new package, and put all alloc'd memory into bin.
 * @param str1
 * @param str2
 * @param file
 * @return
 */
sc Package *new_package(char *in, char *out);
/**<------------------------------------------------------------------------>**/

#endif //STRINGTOSTRING_MAIN_H
