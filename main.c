
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv) {
    sc Package *pkg = new_package(argv[1], argv[2]);
    sc Grid *g = pkg->grid;
    sc Bin *b = pkg->bin;
    sc Writer *wr = pkg->writer;

    for (int i = 0; i < g->w; ++i) {
        for (int j = 0; j < g->h; ++j) {
            if (i == 0 _and j == 0) {
                g->__add_node(g, new_node(0, NON, i, j, NULL, b));
            } else if (i == 0) {
                sc Node *p = g->__get_node(g, i, j - 1);
                g->__add_node(g, new_node(p->stp + 1, RMV, i, j, ref(p), b));
            } else if (j == 0) {
                sc Node *p = g->__get_node(g, i - 1, j);
                g->__add_node(g, new_node(p->stp + 1, INS, i, j, ref(p), b));
            } else {
                if (pkg->str1[j - 1] == pkg->str2[i - 1]) {
                    sc Node *p = g->__get_node(g, i - 1, j - 1);
                    g->__add_node(g, new_node(p->stp, NON, i, j, ref(p), b));
                } else {
                    sc Node *p = min_node(
                            g->__get_node(g, i - 1, j - 1),
                            g->__get_node(g, i, j - 1),
                            g->__get_node(g, i - 1, j)
                    );
                    en Op op;
                    if (p->__equals(p, g->__get_node(g, i, j - 1))) {
                        op = RMV;
                    } else if (p->__equals(p, g->__get_node(g, i - 1, j))) {
                        op = INS;
                    } else {
                        op = REP;
                    }

                    g->__add_node(g, new_node(p->stp + 1, op, i, j, ref(p), b));
                }
            }
        }
    }

    sc Node *root = g->__get_node(g, g->w - 1, g->h - 1);
    while (root->p != NULL) {
        wr->__write_line(wr, root->__to_string(root, pkg->str2, b));
        root = root->p;
    }

    wr->__close(wr);
    b->__take_out_the_trash(ref(b));
    return 0;
}



/**<--------------------               Bin              -------------------->**/
sc Bin *new_bin(void) {
    struct Bin *bin = malloc(sizeof(sc Bin));
    bin->capacity = BUFFER_SIZE;
    bin->size = 0;
    bin->list = malloc(sizeof(void *) * bin->capacity);

    bin->__trash_this_ptr = add_to_bin;
    bin->__take_out_the_trash = remove_all;
    bin->_get_bigger_bin = increase_bin_size;

    return bin;
}

void add_to_bin(sc Bin *self, void *ptr) {
    if (self->size == self->capacity) {
        self->_get_bigger_bin(self);
    } else {
        self->list[self->size++] = ptr;
    }
}

void increase_bin_size(sc Bin *self) {
    self->capacity *= 2;
    self->list = realloc(self->list, sizeof(void *) * self->capacity);
}

void remove_all(sc Bin **self) {
    sc Bin *b = deref(self);
    for (int i = 0; i < b->size; i++) {
        free(b->list[i]);
    };

    free(b->list);
    free(b);
}
/**<------------------------------------------------------------------------>**/

/**<--------------------              Grid              -------------------->**/
sc Grid *new_grid(long width, long height, sc Bin *bin) {
    sc Grid *grid = malloc(sizeof(sc Grid));
    bin->__trash_this_ptr(bin, grid);

    grid->w = width;
    grid->h = height;
    grid->list = malloc(sizeof(sc Node *) * width);
    bin->__trash_this_ptr(bin, grid->list);
    for (int i = 0; i < grid->w; ++i) {
        grid->list[i] = malloc(sizeof(sc Node) * height);
        bin->__trash_this_ptr(bin, grid->list[i]);
    }

    grid->__add_node = add_node;
    grid->__get_node = get_node;

    return grid;
}

void add_node(sc Grid *self, sc Node *node) {
    self->list[node->i][node->j] = deref(node);
}

sc Node *get_node(sc Grid *self, int i, int j) {
    return ref(self->list[i][j]);
}
/**<------------------------------------------------------------------------>**/

/**<--------------------              Node              -------------------->**/
sc Node *new_node(int steps, en Op op, int i, int j,
                  sc Node **parent, sc Bin *bin) {
    sc Node *node = malloc(sizeof(sc Node));
    bin->__trash_this_ptr(bin, node);

    node->p = (parent != NULL) ? deref(parent) : NULL;
    node->op = op;
    node->stp = steps;
    node->i = i;
    node->j = j;

    node->__equals = node_equals;
    node->__to_string = node_to_string;

    return node;
}

sc Node *min_node(sc Node *n1, sc Node *n2, sc Node *n3) {
    sc Node *n = (n1->stp <= n2->stp) ? n1 : n2;
    return (n->stp <= n3->stp) ? n : n3;
}

int node_equals(sc Node *self, sc Node *node) {
    return (self->i == node->i _and self->j == node->j) ? 1 : 0;
}

char *node_to_string(sc Node *self, char *str, sc Bin *bin) {
    char *line = malloc(sizeof(char) * BUFFER_SIZE); // Lets hope we dont ovrflw
    bin->__trash_this_ptr(bin, line);

    switch(self->op) {
        case RMV:
            sprintf(line, "d:%d\n", self->j - 1);
            break;
        case INS:
            sprintf(line, "i:%d:%c\n", self->j, str[self->i - 1]);
            break;
        case REP:
            sprintf(line, "c:%d:%c\n", self->j - 1, str[self->i - 1]);
            break;
        case NON:
            return NULL;
    }

    return line;
}
/**<------------------------------------------------------------------------>**/

/**<--------------------             Writer             -------------------->**/
sc Writer *new_writer(char *file, sc Bin *bin) {
    sc Writer *writer = malloc(sizeof(sc Writer));
    bin->__trash_this_ptr(bin, writer);

    writer->file = fopen(file, "w");

    writer->__write_line = write_line;
    writer->__close = close_writer;

    return writer;
}

void write_line(sc Writer *self, char *line) {
    if (line != NULL) { fputs(line, self->file); }
}

void close_writer(sc Writer *self) {
    fclose(self->file);
}
/**<------------------------------------------------------------------------>**/

/**<--------------------             Reader             -------------------->**/
sc Reader *new_reader(char *file, sc Bin *bin) {
    sc Reader *reader = malloc(sizeof(sc Reader));
    bin->__trash_this_ptr(bin, reader);

    reader->file = fopen(file, "r");

    reader->__next_line = read_line;
    reader->__close = close_reader;

    return reader;
}

char *read_line(sc Reader *self, sc Bin *bin) {
    int i = 0;
    int size = BUFFER_SIZE;
    char *line = malloc(sizeof(char) * size);

    for (_ever) {
        int c = fgetc(self->file);
        if (i == size) {
            size *= 2;
            line = realloc(line, size);
        }

        if (feof(self->file) _or (char) c == '\n') {
            line[i] = '\0';
            break;
        }

        line[i++] = (char) c;
    }

    bin->__trash_this_ptr(bin, line);
    return line;
}

void close_reader(sc Reader *self) {
    fclose(self->file);
}
/**<------------------------------------------------------------------------>**/

/**<--------------------            Package             -------------------->**/
sc Package *new_package(char *in, char *out) {
    sc Package *pkg = malloc(sizeof(sc Package));
    pkg->bin = new_bin();
    pkg->bin->__trash_this_ptr(pkg->bin, pkg);

    pkg->reader = new_reader(in, pkg->bin);
    pkg->writer = new_writer(out, pkg->bin);

    pkg->str1 = pkg->reader->__next_line(pkg->reader, pkg->bin);
    pkg->str2 = pkg->reader->__next_line(pkg->reader, pkg->bin);
    pkg->reader->__close(pkg->reader);

    pkg->grid = new_grid(strlen(pkg->str2) + 1,
                         strlen(pkg->str1) + 1, pkg->bin);

    return pkg;
}
/**<------------------------------------------------------------------------>**/