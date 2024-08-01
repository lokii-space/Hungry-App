#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    TRUE,
    FALSE
} boolean;

typedef enum
{
    SUCCESS,
    FAILURE
} statuscode;

typedef struct date_tag
{
    int day;
    int month;
    int year;
} date;

date today;

// RESTAURANT LIST

typedef struct restaurant_tag
{
    char *name;
    char *category;
    int no_of_seats;
    char *special_facilities;
    char *area;
    char *menu;
    char *type_of_cuisine;
    char *nearby_area;
    struct restaurant_tag *left;
    struct restaurant_tag *right;
    int height;

} restaurant_node;

// AGENT LIST

typedef struct agent_tag
{
    boolean is_available;
    int agent_id;
    char *name;
    int phone_no;
    int commission;
    char *area;
    struct agent_tag *left;
    struct agent_tag *right;
    int height;

} agent_node;

// ORDER NODE

typedef struct order_tag
{
    int order_id;
    int user_id;
    char *custumer_name;
    char *customer_address;
    int customer_phone_no;
    char *eating_joint;
    char *ordered_menu;
    int user_count;
    int order_count;
    int res_count;
    date d;
    agent_node *assignned_agent;
    struct order_tag *left;
    struct order_tag *right;
    int height;

} order_node;

// USER LIST

typedef struct user_tag
{
    int user_id;
    char *name;
    int phone_no;
    char *address;
    order_node *order_history;
    struct user_tag *left;
    struct user_tag *right;
    int height;

} user_node;

int max(int a, int b)
{
    return (a > b) ? a : b;
}

// GLOBAL DATABASES

restaurant_node *res_root = NULL;
agent_node *agent_root = NULL;
user_node *user_root = NULL;
order_node *order_root = NULL;      // global orders
order_node *all_orders_root = NULL; // all orders

// RESTAURANT NODE

int getheight(restaurant_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

restaurant_node *create_restaurant_node(char *name, char *category, int no_of_seats, char *area, char *menu, char *cuisine, char *nearby_area, char *special_facilities)
{
    restaurant_node *new = (restaurant_node *)malloc(sizeof(restaurant_node));

    new->name = strdup(name);
    new->category = strdup(category);
    new->no_of_seats = no_of_seats;
    new->area = strdup(area);
    new->menu = strdup(menu);
    new->type_of_cuisine = strdup(cuisine);
    new->nearby_area = strdup(nearby_area);
    new->special_facilities = strdup(special_facilities);
    new->left = NULL;
    new->right = NULL;
    new->height = 1;

    return new;
}

int getBalancefactor(restaurant_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getheight(n->left) - getheight(n->right);
}

restaurant_node *RotateLeft(restaurant_node *r)
{
    restaurant_node *x;
    if (r == NULL)
    {
        printf("ERROR");
    }
    else if (r->right == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = r->right;
        r->right = x->left;
        x->left = r;
    }

    r->height = max(getheight(r->left), getheight(r->right)) + 1;
    x->height = max(getheight(x->left), getheight(x->right)) + 1;

    return x;
}

restaurant_node *RotateRight(restaurant_node *r)
{
    restaurant_node *x;
    if (r == NULL)
    {
        printf("ERROR");
    }
    else if (r->left == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = r->left;
        r->left = x->right;
        x->right = r;
    }

    r->height = max(getheight(r->left), getheight(r->right)) + 1;
    x->height = max(getheight(x->left), getheight(x->right)) + 1;

    return x;
}

restaurant_node *insert(restaurant_node *res_root, restaurant_node *res)
{
    if (res_root == NULL)
    {
        res_root = res;
        return res_root;
    }

    if (strcmp(res->name, res_root->name) < 0)
    {
        res_root->left = insert(res_root->left, res);
    }
    else if (strcmp(res->name, res_root->name) > 0)
    {
        res_root->right = insert(res_root->right, res);
    }

    res_root->height = 1 + max(getheight(res_root->left), getheight(res_root->right));
    int bf = getBalancefactor(res_root);

    // LL insertion CASE-->Right rotation
    if (bf > 1 && strcmp(res->name, res_root->left->name) < 0)
    {
        res_root = RotateRight(res_root);
    }

    // RR insertion case-->Left rotation
    if (bf < -1 && strcmp(res->name, res_root->right->name) > 0)
    {
        res_root = RotateLeft(res_root);
    }

    // LR insertion case-->
    if (bf > 1 && strcmp(res->name, res_root->left->name) > 0)
    {
        res_root->left = (res_root->left);
        res_root = RotateRight(res_root);
    }

    // RL insertion case-->
    if (bf < -1 && strcmp(res->name, res_root->right->name) < 0)
    {
        res_root->right = RotateRight(res_root->right);
        res_root = RotateLeft(res_root);
    }

    return res_root;
}

void read_restaurant_data(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), fp))
    {
        char name[50], address[50], category[50], menu[50], cuisine[50], nearby_area[50], special_fac[50];
        int seats;

        sscanf(line, "%s %s %d %s %s %s %s %s", name, category, &seats, address, menu, cuisine, nearby_area, special_fac);

        restaurant_node *new_restaurant = create_restaurant_node(name, category, seats, address, menu, cuisine, nearby_area, special_fac);

        res_root = insert(res_root, new_restaurant);
    }
    fclose(fp);
}

// USER NODE

int getheight_user(user_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

user_node *create_user_node(int id, char *name, char *addr, int phone_no)
{
    user_node *new = (user_node *)malloc(sizeof(user_node));

    new->name = strdup(name);
    new->user_id = id;
    new->address = strdup(addr);
    new->phone_no = phone_no;
    new->order_history = NULL;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;

    return new;
}

int getBalancefactor_user(user_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getheight_user(n->left) - getheight_user(n->right);
}

user_node *RotateLeft_user(user_node *r)
{
    user_node *x;
    if (r == NULL)
    {
        printf("ERROR");
    }
    else if (r->right == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = r->right;
        r->right = x->left;
        x->left = r;
    }

    r->height = max(getheight_user(r->left), getheight_user(r->right)) + 1;
    x->height = max(getheight_user(x->left), getheight_user(x->right)) + 1;

    return x;
}

user_node *RotateRight_user(user_node *r)
{
    user_node *x;
    if (r == NULL)
    {
        printf("ERROR");
    }
    else if (r->left == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = r->left;
        r->left = x->right;
        x->right = r;
    }

    r->height = max(getheight_user(r->left), getheight_user(r->right)) + 1;
    x->height = max(getheight_user(x->left), getheight_user(x->right)) + 1;

    return x;
}

user_node *insert_user(user_node *user_root, user_node *user)
{
    if (user_root == NULL)
    {
        user_root = user;
        return user_root;
    }

    if (user->user_id < user_root->user_id)
    {
        user_root->left = insert_user(user_root->left, user);
    }
    else if (user->user_id > user_root->user_id)
    {
        user_root->right = insert_user(user_root->right, user);
    }

    user_root->height = 1 + max(getheight_user(user_root->left), getheight_user(user_root->right));
    int bf = getBalancefactor_user(user_root);

    // LL insertion CASE-->Right rotation
    if (bf > 1 && user->user_id < user_root->left->user_id)
    {
        user_root = RotateRight_user(user_root);
    }

    // RR insertion case-->Left rotation
    if (bf < -1 && user->user_id > user_root->right->user_id)
    {
        user_root = RotateLeft_user(user_root);
    }

    // LR insertion case-->
    if (bf > 1 && user->user_id > user_root->left->user_id)
    {
        user_root->left = (user_root->left);
        user_root = RotateRight_user(user_root);
    }

    // RL insertion case-->
    if (bf < -1 && user->user_id < user_root->right->user_id)
    {
        user_root->right = RotateRight_user(user_root->right);
        user_root = RotateLeft_user(user_root);
    }

    return user_root;
}

void read_user_data(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }
    char line[1000];
    while (fgets(line, sizeof(line), fp))
    {
        char name[50], addr[50];
        int id, phone_no;

        sscanf(line, "%s %s %d %d", name, addr, &id, &phone_no);

        user_node *new = create_user_node(id, name, addr, phone_no);

        user_root = insert_user(user_root, new);
    }
    fclose(fp);
}

// AGENT NODE

int getheight_agent(agent_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

agent_node *create_agent_node(int id, char *name, int phone_no, char *area)
{
    agent_node *new = (agent_node *)malloc(sizeof(agent_node));

    new->agent_id = id;
    new->name = strdup(name);
    new->area = strdup(area);
    new->phone_no = phone_no;
    new->is_available = TRUE;
    new->commission = 0;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;

    return new;
}

int getBalancefactor_agent(agent_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getheight_agent(n->left) - getheight_agent(n->right);
}

agent_node *RotateLeft_agent(agent_node *a)
{
    agent_node *x;
    if (a == NULL)
    {
        printf("ERROR");
    }
    else if (a->right == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = a->right;
        a->right = x->left;
        x->left = a;
    }

    a->height = max(getheight_agent(a->left), getheight_agent(a->right)) + 1;
    x->height = max(getheight_agent(x->left), getheight_agent(x->right)) + 1;

    return x;
}

agent_node *RotateRight_agent(agent_node *a)
{
    agent_node *x;
    if (a == NULL)
    {
        printf("ERROR");
    }
    else if (a->left == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = a->left;
        a->left = x->right;
        x->right = a;
    }

    a->height = max(getheight_agent(a->left), getheight_agent(a->right)) + 1;
    x->height = max(getheight_agent(x->left), getheight_agent(x->right)) + 1;

    return x;
}

agent_node *insert_agent(agent_node *agent_root, agent_node *agt)
{
    if (agent_root == NULL)
    {
        agent_root = agt;
        return agent_root;
    }

    if (agt->agent_id < agent_root->agent_id)
    {
        agent_root->left = insert_agent(agent_root->left, agt);
    }
    else if (agt->agent_id > agent_root->agent_id)
    {
        agent_root->right = insert_agent(agent_root->right, agt);
    }

    agent_root->height = 1 + max(getheight_agent(agent_root->left), getheight_agent(agent_root->right));
    int bf = getBalancefactor_agent(agent_root);

    // LL insertion CASE-->Right rotation
    if (bf > 1 && agt->agent_id < agent_root->left->agent_id)
    {
        agent_root = RotateRight_agent(agent_root);
    }

    // RR insertion case-->Left rotation
    if (bf < -1 && agt->agent_id > agent_root->right->agent_id)
    {
        agent_root = RotateLeft_agent(agent_root);
    }

    // LR insertion case-->
    if (bf > 1 && agt->agent_id > agent_root->left->agent_id)
    {
        agent_root->left = (agent_root->left);
        agent_root = RotateRight_agent(agent_root);
    }

    // RL insertion case-->
    if (bf < -1 && agt->agent_id < agent_root->right->agent_id)
    {
        agent_root->right = RotateRight_agent(agent_root->right);
        agent_root = RotateLeft_agent(agent_root);
    }

    return agent_root;
}

void read_agent_data(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }

    char line[1000];

    while (fgets(line, sizeof(line), fp))
    {
        char name[50], area[50];
        int id, phone_no;

        sscanf(line, "%s %d %d %s", name, &id, &phone_no, area);

        agent_node *new = create_agent_node(id, name, phone_no, area);

        agent_root = insert_agent(agent_root, new);
    }
    fclose(fp);
}

// ORDER

int getheight_order(order_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

order_node *create_new_order(int order_id, int user_id, char *name, char *address, int phone_no, char *eating_joint, char *ordered_menu, date d)
{
    order_node *new = (order_node *)malloc(sizeof(order_node));

    new->order_id = order_id;
    new->user_id = user_id;
    new->custumer_name = strdup(name);
    new->customer_address = strdup(address);
    new->customer_phone_no = phone_no;
    new->eating_joint = strdup(eating_joint);
    new->ordered_menu = strdup(ordered_menu);
    new->d = d;
    new->assignned_agent = NULL;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;

    return new;
}

int getBalancefactor_order(order_node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getheight_order(n->left) - getheight_order(n->right);
}

order_node *RotateLeft_order(order_node *o)
{
    order_node *x;
    if (o == NULL)
    {
        printf("ERROR");
    }
    else if (o->right == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = o->right;
        o->right = x->left;
        x->left = o;
    }

    o->height = max(getheight_order(o->left), getheight_order(o->right)) + 1;
    x->height = max(getheight_order(x->left), getheight_order(x->right)) + 1;

    return x;
}

order_node *RotateRight_order(order_node *o)
{
    order_node *x;
    if (o == NULL)
    {
        printf("ERROR");
    }
    else if (o->left == NULL)
    {
        printf("ERROR");
    }
    else
    {
        x = o->left;
        o->left = x->right;
        x->right = o;
    }

    o->height = max(getheight_order(o->left), getheight_order(o->right)) + 1;
    x->height = max(getheight_order(x->left), getheight_order(x->right)) + 1;

    return x;
}

order_node *insert_order(order_node *order_root, order_node *ord)
{
    if (order_root == NULL)
    {
        order_root = ord;

        return ord;
    }

    if (ord->order_id < order_root->order_id)
    {
        order_root->left = insert_order(order_root->left, ord);
    }
    else if (ord->order_id > order_root->order_id)
    {
        order_root->right = insert_order(order_root->right, ord);
    }

    order_root->height = 1 + max(getheight_order(order_root->left), getheight_order(order_root->right));
    int bf = getBalancefactor_order(order_root);

    // LL insertion CASE-->Right rotation
    if (bf > 1 && ord->order_id < order_root->left->order_id)
    {
        order_root = RotateRight_order(order_root);
    }

    // RR insertion case-->Left rotation
    if (bf < -1 && ord->order_id > order_root->right->order_id)
    {
        order_root = RotateLeft_order(order_root);
    }

    // LR insertion case-->
    if (bf > 1 && ord->order_id > order_root->left->order_id)
    {
        order_root->left = (order_root->left);
        order_root = RotateRight_order(order_root);
    }

    // RL insertion case-->
    if (bf < -1 && ord->order_id < order_root->right->order_id)
    {
        order_root->right = RotateRight_order(order_root->right);
        order_root = RotateLeft_order(order_root);
    }

    return order_root;
}

/*
1. Different Search() functions to search for eating locations;
• Based on category (restaurant, cafe, pub etc)
• Based on category of food or cuisine (north Indian, south Indian, continental etc)
• Based on area (Name of area and its nearby areas). Note that you can keep
information which tells which other areas of city are near the given area.
*/

void search_by_category(restaurant_node *root, char *ctg)
{
    if (root == NULL)
    {
        return;
    }
    if (root != NULL && strcmp(root->category, ctg) == 0)
    {
        printf("Name: %s\nAddress: %s\nNo of seats: %d\nMenu: %s\nSpecial facilities: %s\n\n", root->name, root->area, root->no_of_seats, root->menu, root->special_facilities);
    }

    search_by_category(root->left, ctg);
    search_by_category(root->right, ctg);
}

void search_by_cuisine(restaurant_node *root, char *cuisine)
{
    if (root == NULL)
    {
        return;
    }
    if (root != NULL && strcmp(root->type_of_cuisine, cuisine) == 0)
    {
        printf("Name: %s\nAddress: %s\nNo of seats: %d\nMenu: %s\nSpecial facilities: %s\n\n", root->name, root->area, root->no_of_seats, root->menu, root->special_facilities);
    }

    search_by_cuisine(root->left, cuisine);
    search_by_cuisine(root->right, cuisine);
}

void search_by_area(restaurant_node *root, char *area)
{
    if (root == NULL)
    {
        return;
    }
    if (root != NULL && strcmp(root->area, area) == 0)
    {
        printf("Name: %s\nAddress: %s\nNo of seats: %d\nMenu: %s\nSpecial facilities: %s\n\n", root->name, root->area, root->no_of_seats, root->menu, root->special_facilities);
    }

    search_by_area(root->left, area);
    search_by_area(root->right, area);
}

/*
2.Order (customer name, address, Phone No., Eating-joint, Menu);
Each order should be added to the global list of orders which maintain all the orders
yet to be delivered. This function allocates an available agent to this particular order.
The output of this function should be order details along with agent details. You can
decide the sorted order you want to store the global list of orders so that it is easy for
different operations.
*/

void preOrder(order_node *root)
{
    if (root != NULL)
    {
        printf("%s----%d\n", root->ordered_menu, root->order_count);

        preOrder(root->left);

        preOrder(root->right);
    }
}

agent_node *find_available_agent(agent_node *root)
{
    if (root == NULL)
    {
        return NULL; // no available agents found
    }
    else if (root->is_available == TRUE)
    {
        return root; // available agent found
    }
    else
    {
        agent_node *retval = find_available_agent(root->left); // search in left subtree
        if (retval == NULL)
        {
            retval = find_available_agent(root->right); // search in right subtree
        }
        return retval;
    }
}

void search_for_foodcount(order_node *root, order_node *o)
{
    // order_node* retval;
    if (root == NULL)
    {
        return;
    }

    if (strcmp(root->ordered_menu, o->ordered_menu) == 0)
    {
        // printf("root count- %d\n", root->order_count);
        root->order_count += 1;
        o->order_count = root->order_count;
        // printf("new root count- %d\n", root->order_count);
        // printf("new order count- %d\n", o->order_count);
    }

    search_for_foodcount(root->left, o);
    search_for_foodcount(root->right, o);
}

void search_for_rescount(order_node *root, order_node *o)
{
    // order_node* retval;
    if (root == NULL)
    {
        return;
    }

    if (strcmp(root->eating_joint, o->eating_joint) == 0)
    {
        // printf("root count- %d\n", root->res_count);
        root->res_count += 1;
        o->res_count = root->res_count;
        // printf("new root count- %d\n", root->res_count);
        // printf("new order count- %d\n", o->res_count);
    }

    search_for_rescount(root->left, o);
    search_for_rescount(root->right, o);
}

void search_for_usercount(order_node *root, order_node *o)
{
    if (root == NULL)
    {
        return;
    }

    if (strcmp(root->ordered_menu, o->ordered_menu) == 0)
    {
        // printf("root count- %d\n", root->order_count);
        root->user_count += 1;
        o->user_count = root->user_count;
        // printf("new root count- %d\n", root->order_count);
        // printf("new order count- %d\n", o->order_count);
    }

    search_for_usercount(root->left, o);
    search_for_usercount(root->right, o);
}

void order(int id, int u_id, char *customer_name, char *customer_address, int phone_no, char *restaurant_name, char *menu, date d)
{
    order_node *new_order;
    new_order = create_new_order(id, u_id, customer_name, customer_address, phone_no, restaurant_name, menu, d);
    new_order->order_count = 1;
    new_order->res_count = 1;

    agent_node *ptr;

    ptr = find_available_agent(agent_root);

    if (ptr == NULL)
    {
        printf("No agent available\n");
    }
    else
    {
        new_order->assignned_agent = ptr;
        ptr->is_available = FALSE;

        // now need to check for order_count and res_count

        search_for_foodcount(order_root, new_order);

        search_for_rescount(order_root, new_order);

        // adding order in global list

        order_root = insert_order(order_root, new_order);

        // now adding the order in all orders list

        order_node *new = (order_node *)malloc(sizeof(order_node));
        new = create_new_order(id, u_id, customer_name, customer_address, phone_no, restaurant_name, menu, d);
        new->order_count = 1;
        new->res_count = 1;

        search_for_foodcount(all_orders_root, new);

        search_for_rescount(all_orders_root, new);

        all_orders_root = insert_order(all_orders_root, new);

        // now finding that user to add order in his order history
        user_node *temp = user_root;

        int flag = 0;

        while (temp != NULL && strcmp(temp->name, new_order->custumer_name) != 0)
        {
            if (temp->user_id > new_order->user_id)
            {
                temp = temp->left;
            }
            else
            {
                temp = temp->right;
            }
        }

        // temp is our user now adding this order to his order history

        // order_node *x = temp->order_history; // this is root
        order_node *n = (order_node *)malloc(sizeof(order_node));
        n = create_new_order(id, u_id, customer_name, customer_address, phone_no, restaurant_name, menu, d);
        n->user_count = 1;
        search_for_usercount(temp->order_history, n);

        temp->order_history = insert_order(temp->order_history, n);

        // printing

        printf("\n");

        printf("ORDER DETAILS:\n\n");

        printf("Order id: %d\n", new_order->order_id);
        printf("Customer Name: %s\n", new_order->custumer_name);
        printf("Customer Address: %s\n", new_order->customer_address);
        printf("Customer Phone no: %d\n", new_order->customer_phone_no);
        printf("Restaurant name: %s\n", new_order->eating_joint);
        printf("Ordered food: %s\n", new_order->ordered_menu);

        printf("\n");

        printf("AGENT DETAILS:\n");
        printf("Agent id: %d\n", new_order->assignned_agent->agent_id);
        printf("Agent name: %s\n", new_order->assignned_agent->name);
        printf("Agent phone no: %d\n", new_order->assignned_agent->phone_no);
        printf("Commission: %d\n", new_order->assignned_agent->commission);

        printf("\n\n");
    }
}

/*
3.Delivery() : This function is to be executed on the actual delivery and the order from
global  order  list  is  removed  which  also  frees  the  respective  agent.  It  also  adds  10%
commission to the particular agent’s currently-accumulated commission.
*/

order_node *minValueNode(order_node *node)
{
    order_node *current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

order_node *deleteNode(order_node *order_root, int key)
{
    if (order_root == NULL)
        return order_root;

    if (key < order_root->order_id)
        order_root->left = deleteNode(order_root->left, key);

    else if (key > order_root->order_id)
        order_root->right = deleteNode(order_root->right, key);

    else
    {
        if ((order_root->left == NULL) || (order_root->right == NULL))
        {
            order_node *temp = order_root->left ? order_root->left : order_root->right;

            if (temp == NULL)
            {
                temp = order_root;
                order_root = NULL;
            }
            else
                *order_root = *temp;
            free(temp);
        }
        else
        { // 2 child case
            order_node *temp = minValueNode(order_root->right);

            order_root->order_id = temp->order_id;
            order_root->custumer_name = strdup(temp->custumer_name);
            order_root->customer_address = strdup(temp->customer_address);
            order_root->customer_phone_no = temp->customer_phone_no;
            order_root->eating_joint = strdup(temp->eating_joint);
            order_root->ordered_menu = strdup(temp->ordered_menu);
            order_root->d = temp->d;
            order_root->assignned_agent = temp->assignned_agent;
            order_root->order_count = temp->order_count;
            order_root->res_count = temp->res_count;
            order_root->user_count = temp->user_count;

            order_root->right = deleteNode(order_root->right, temp->order_id);
        }
    }

    if (order_root == NULL)
        return order_root;

    order_root->height = 1 + max(getheight_order(order_root->left),
                                 getheight_order(order_root->right));

    int balance = getBalancefactor_order(order_root);

    if (balance > 1 && getBalancefactor_order(order_root->left) >= 0)
        return RotateRight_order(order_root);

    if (balance > 1 && getBalancefactor_order(order_root->left) < 0)
    {
        order_root->left = RotateLeft_order(order_root->left);
        return RotateRight_order(order_root);
    }

    if (balance < -1 && getBalancefactor_order(order_root->right) <= 0)
        return RotateLeft_order(order_root);

    if (balance < -1 && getBalancefactor_order(order_root->right) > 0)
    {
        order_root->right = RotateRight_order(order_root->right);
        return RotateLeft_order(order_root);
    }

    return order_root;
}

void print_live_orders(order_node *order_root);

void delivery(int order_id)
{
    // list before delivery
    printf("BEFORE DELIVERY\n");
    print_live_orders(order_root);

    order_node *ptr = order_root;

    while (ptr != NULL && ptr->order_id != order_id)
    {
        if (ptr->order_id > order_id)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }

    // now ptr is our order to be delivered

    ptr->assignned_agent->is_available = TRUE;
    int x = ptr->assignned_agent->commission;
    x = x + 0.1 * x;
    ptr->assignned_agent->commission = x;

    order_root = deleteNode(order_root, ptr->order_id);

    printf("\n");
    printf("AFTER DELIVERY\n");
    print_live_orders(order_root);
}

/*
4.CancelOrder() : It cancels the order, removes it from global list of orders and frees
the agent.
*/

void cancel(int order_id)
{
    // list before cancelling
    printf("BEFORE CANCELLING\n");
    print_live_orders(order_root);

    order_node *ptr = order_root;

    while (ptr != NULL && ptr->order_id != order_id)
    {
        if (ptr->order_id > order_id)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }

    // now ptr is our order to be cancelled

    ptr->assignned_agent->is_available = TRUE;

    order_root = deleteNode(order_root, ptr->order_id);

    // list after cancelling
    printf("AFTER CANCELLING\n");
    print_live_orders(order_root);
}

/*
5.FindFavoriteFoodsOfUser(user-id) : For the given user-id, find top 3 favorite food
items based on previous orders of the last 3 months
*/

char *top3[3];
int max1 = 0;
int max2 = 0;
int max3 = 0;

void fav_foodsuser_helper(order_node *root, char *top3[])
{
    if (root == NULL)
    {
        return;
    }

    fav_foodsuser_helper(root->left, top3);

    if (root != NULL)
    {
        if (root->d.year == today.year)
        {
            int diff = today.month - root->d.month;
            if (diff <= 3)
            {
                if (root->user_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->user_count;

                    top3[2] = top3[1];
                    top3[1] = top3[0];
                    top3[0] = root->ordered_menu;
                }
                else if (root->user_count < max1 && root->user_count > max2)
                {
                    max3 = max2;
                    max2 = root->user_count;

                    top3[2] = top3[1];
                    top3[1] = root->ordered_menu;
                }
                else if (root->user_count < max2 && root->user_count > max3)
                {
                    max3 = root->user_count;

                    top3[2] = root->ordered_menu;
                }
            }
        }
        else
        {
            int diff = 12 - root->d.month + today.month;
            if (diff <= 3)
            {
                if (root->user_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->user_count;

                    top3[2] = top3[1];
                    top3[1] = top3[0];
                    top3[0] = root->ordered_menu;
                }
                else if (root->user_count < max1 && root->user_count > max2)
                {
                    max3 = max2;
                    max2 = root->user_count;

                    top3[2] = top3[1];
                    top3[1] = root->ordered_menu;
                }
                else if (root->user_count < max2 && root->user_count > max3)
                {
                    max3 = root->user_count;

                    top3[2] = root->ordered_menu;
                }
            }
        }
    }

    fav_foodsuser_helper(root->right, top3);
}

void find_favourite_foods_of_user(int id)
{
    user_node *temp = user_root;
    int flag = 0;

    while (temp != NULL && temp->user_id != id)
    {
        if (temp->user_id > id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }

    // temp is our user
    // go to his order history

    order_node *ptr = temp->order_history; // root of order history of user now traversing
    // preOrder(ptr);
    max1 = 0, max2 = 0, max3 = 0;
    top3[0] = "";
    top3[1] = "";
    top3[2] = "";
    fav_foodsuser_helper(temp->order_history, top3);

    // printing the top 3 ordered food accross restaurants in last N days
    printf("TOP 3 FAVOURITE FOODS OF USER %d:\n", id);

    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", top3[i]);
    }
}

/*
6.FindFavoriteRestaurants(int N) : Finds the top-3 restaurants that have got
maximum orders in the last N days.
*/

char *topres[3];

void fav_restuarants_helper(order_node *root, char *topres[], int N)
{
    if (root == NULL)
    {
        return;
    }

    fav_restuarants_helper(root->left, topres, N);

    if (root != NULL)
    {
        if (today.month == root->d.month)
        {
            if (today.day - root->d.day <= N)
            {
                if (root->res_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->res_count;

                    topres[2] = topres[1];
                    topres[1] = topres[0];
                    topres[0] = root->eating_joint;
                }
                else if (root->res_count < max1 && root->res_count > max2)
                {
                    max3 = max2;
                    max2 = root->res_count;

                    topres[2] = topres[1];
                    topres[1] = root->eating_joint;
                }
                else if (root->res_count < max2 && root->res_count > max3)
                {
                    max3 = root->res_count;

                    topres[2] = root->eating_joint;
                }
            }
        }
        else
        {
            if (((today.month - root->d.month) * 30 + (today.day - root->d.day)) <= N)
            {
                if (root->res_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->res_count;

                    topres[2] = topres[1];
                    topres[1] = topres[0];
                    topres[0] = root->eating_joint;
                }
                else if (root->res_count < max1 && root->res_count > max2)
                {
                    max3 = max2;
                    max2 = root->res_count;

                    topres[2] = topres[1];
                    topres[1] = root->eating_joint;
                }
                else if (root->res_count < max2 && root->res_count > max3)
                {
                    max3 = root->res_count;

                    topres[2] = root->eating_joint;
                }
            }
        }

        fav_restuarants_helper(root->right, topres, N);
    }
}

void find_favourite_restaurants(int N)
{
    order_node *ptr = all_orders_root;

    max1 = 0, max2 = 0, max3 = 0;
    // preOrder(all_orders_root);

    fav_restuarants_helper(all_orders_root, topres, N);

    // printing the top 3 ordered food accross restaurants in last N days
    printf("TOP 3 RESTAURANTS IN LAST %d DAYS:\n", N);

    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", topres[i]);
    }
}

/*
7. FindFavoriteFoodsAcrossRestaurants(int N) : Finds the top-3 ordered most
favorite food items in the last N days.
*/

char *topfood[3];

void fav_foods_helper(order_node *root, char *topfood[], int N)
{
    if (root == NULL)
    {
        return;
    }

    fav_foods_helper(root->left, topfood, N);

    if (root != NULL)
    {
        if (today.month == root->d.month)
        {
            if (today.day - root->d.day <= N)
            {
                if (root->order_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->order_count;

                    topfood[2] = topfood[1];
                    topfood[1] = topfood[0];
                    topfood[0] = root->ordered_menu;
                }
                else if (root->order_count < max1 && root->order_count > max2)
                {
                    max3 = max2;
                    max2 = root->order_count;

                    topfood[2] = topfood[1];
                    topfood[1] = root->ordered_menu;
                }
                else if (root->order_count < max2 && root->order_count > max3)
                {
                    max3 = root->order_count;

                    topfood[2] = root->ordered_menu;
                }
            }
        }
        else
        {
            if (((today.month - root->d.month) * 30 + (today.day - root->d.day)) <= N)
            {
                if (root->order_count > max1)
                {
                    max3 = max2;
                    max2 = max1;
                    max1 = root->order_count;

                    topfood[2] = topfood[1];
                    topfood[1] = topfood[0];
                    topfood[0] = root->ordered_menu;
                }
                else if (root->order_count < max1 && root->order_count > max2)
                {
                    max3 = max2;
                    max2 = root->order_count;

                    topfood[2] = topfood[1];
                    topfood[1] = root->ordered_menu;
                }
                else if (root->order_count < max2 && root->order_count > max3)
                {
                    max3 = root->order_count;

                    topfood[2] = root->ordered_menu;
                }
            }
        }

        fav_foods_helper(root->right, topfood, N);
    }
}

void find_favourite_food_across_restaurants(int N)
{
    order_node *ptr = all_orders_root;

    max1 = 0, max2 = 0, max3 = 0;
    // preOrder(all_orders_root);
    fav_foods_helper(all_orders_root, topfood, N);

    // printing the top 3 ordered food accross restaurants in last N days
    printf("TOP 3 FAVOURITE FOODS ACROSS RESTAURANTS IN LAST %d DAYS:\n", N);

    for (int i = 0; i < 3; i++)
    {
        printf("%s\n", topfood[i]);
    }
}

/*
8. Range-Search – Create/print a list of current orders for users having user-id between
user-id1 and user-id2.
*/

void range_search(user_node *root, int id1, int id2)
{
    if (root == NULL)
    {
        return;
    }

    range_search(root->left, id1, id2);

    if (root->user_id <= id2 && root->user_id >= id1)
    {
        // printf("%d\n",root->user_id);
        //  print its order history
        order_node *ptr = root->order_history;
        if (root->order_history != NULL)
        {
            printf("Order history of user %d is:\n", root->user_id);
            print_live_orders(root->order_history);
            printf("\n");
        }
    }

    range_search(root->right, id1, id2);
}

/*
• Print agents database and their details
• Print live orders from global database
• Print area wise agents.
• Print restaurant details for the given restaurant name and address.
*/

void print_agent_database(agent_node *root)
{
    if (root == NULL)
    {
        return;
    }

    print_agent_database(root->left);

    printf("Agent id: %d\n", root->agent_id);
    printf("Agent name: %s\n", root->name);
    printf("Agent phone no: %d\n", root->phone_no);
    printf("Agent commission: %d\n", root->commission);
    if (root->is_available == TRUE)
    {
        printf("Agent availibility: TRUE\n");
    }
    else
    {
        printf("Agent availibility: FALSE\n");
    }
    printf("\n");

    print_agent_database(root->right);
}

void print_live_orders(order_node *root)
{
    if (root == NULL)
    {
        return;
    }

    print_live_orders(root->left);

    printf("Order id: %d\n", root->order_id);
    printf("Customer Name: %s\n", root->custumer_name);
    printf("Customer Address: %s\n", root->customer_address);
    printf("Customer Phone no: %d\n", root->customer_phone_no);
    printf("Restaurant name: %s\n", root->eating_joint);
    printf("Ordered food: %s\n", root->ordered_menu);
    printf("\n");

    print_live_orders(root->right);
}
void print_all_res(restaurant_node* root){
    if(root==NULL)return;
    print_all_res(root->left);
    printf("Restaurant Name: %s\n", root->name);
    printf("Category: %s\n", root->category);
    printf("No of Seats: %d\n", root->no_of_seats);
    printf("Restaurant area: %s\n", root->area);
    printf("Menu: %s\n", root->menu);
    printf("Type of cuisine: %s\n", root->type_of_cuisine);
    printf("Nearby area: %s\n", root->nearby_area);
    printf("Special Facilities: %s\n", root->special_facilities);
    printf("\n");
    print_all_res(root->right);

}
void print_area_wise_agents(agent_node *root, char *area)
{
    if (root == NULL)
    {
        return;
    }

    print_area_wise_agents(root->left, area);

    if (strcmp(root->area, area) == 0)
    {
        printf("%s\n", area);
        printf("Agent id: %d\n", root->agent_id);
        printf("Agent name: %s\n", root->name);
        printf("Agent phone no: %d\n", root->phone_no);
        printf("Agent commission: %d\n", root->commission);
        printf("\n");
    }

    print_area_wise_agents(root->right, area);
}

void print_res_details(restaurant_node *root, char *name, char *addr)
{
    if (root == NULL)
    {
        return;
    }

    print_res_details(root->left, name, addr);

    if (strcmp(root->name, name) == 0 && strcmp(root->area, addr) == 0)
    {
        printf("Name: %s\nAddress: %s\nNo of seats: %d\nMenu: %s\nSpecial facilities: %s\n\n", root->name, root->area, root->no_of_seats, root->menu, root->special_facilities);
    }

    print_res_details(root->right, name, addr);
}

void print(restaurant_node *root)
{
    if (root != NULL)
    {
        printf("%s\n", root->name);

        print(root->left);
        print(root->right);
    }
}

void printa(agent_node *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%s\n", root->name);

    printa(root->left);
    printa(root->right);
}

int main()
{
    read_agent_data("agent.txt");
 
    read_user_data("users.txt");

    read_restaurant_data("restaurants.txt");

    today.day = 11;
    today.month = 4;
    today.year = 23;

    date d1;
    d1.day = 1;
    d1.month = 1;
    d1.year = 23;

    date d2;
    d2.day = 2;
    d2.month = 2;
    d2.year = 23;

    date d3;
    d3.day = 3;
    d3.month = 3;
    d3.year = 23;

    int choice;
    char ch[50];
    int n;
    int id1, id2;

    do
    {
        printf("\n********** HUNGRY APP **********\n");
        printf("0. Print all Restaurants\n");
        printf("1. Search by category\n");
        printf("2. Search by cuisine\n");
        printf("3. Search by area\n");
        printf("4. Order\n");
        printf("5. Delivery\n");
        printf("6. Cancel Order\n");
        printf("7. Find favorite foods of user\n");
        printf("8. Find favorite restaurants\n");
        printf("9. Find favorite foods across restaurants\n");
        printf("10. Print agent list\n");
        printf("11. Print live orders\n");
        printf("12. Print area wise agents\n");
        printf("13. Print restaurant details\n");
        printf("14. Range search\n");
        

        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 0:
            printf("Print all Restaurants\n");
            print_all_res(res_root);
            break;
        case 1:
            printf("Enter category to search for from the following:\npub\ncafe\nrestro\nbar\n");
            scanf("%s", ch);
            search_by_category(res_root, ch);
            break;
        case 2:
            printf("Enter cuisine to search for:\nindian\nchinese\nitalian\nthai\nmexican\n");
            scanf("%s", ch);
            search_by_cuisine(res_root, ch);
            break;
        case 3:
            printf("Enter area to search for:\n");
            scanf("%s", ch);
            search_by_area(res_root, ch);
            break;
        case 4:
            
            order(1, 2001, "Rowan", "address1", 9056, "Restaurant1", "menu1", d1);

            order(2, 2001, "Rowan", "address1", 9056, "Restaurant2", "menu2", d1);

            order(3, 2001, "Rowan", "address1", 9056, "Restaurant1", "menu1", d1);

            order(4, 2002, "Nova", "address2", 9056, "Restaurant2", "menu2", d1);

            order(5, 2003, "Amara", "address3", 9056, "Restaurant3", "menu3", d2);

            order(6, 2001, "Rowan", "address1", 9056, "Restaurant4", "menu4", d2);

            order(7, 2005, "Everly", "address5", 9056, "Restaurant5", "menu5", d2);

            order(8, 2002, "Nova", "address2", 9056, "Restaurant3", "menu3", d3);

            order(9, 2002, "Nova", "address2", 9056, "Restaurant2", "menu2", d3);

            order(10, 2004, "Julian", "address5", 9056, "Restaurant1", "menu1", d3);

            order(11, 2001, "Rowan", "address1", 9056, "Restaurant1", "menu1", d1);

            order(12, 2001, "Rowan", "address1", 9056, "Restaurant2", "menu2", d1);

            order(13, 2002, "Nova", "address2", 9056, "Restaurant2", "menu2", d1);

            order(14, 2002, "Nova", "address2", 9056, "Restaurant4", "menu4", d3);

            order(15, 2002, "Nova", "address2", 9056, "Restaurant4", "menu4", d2);

            break;
        case 5:
            printf("Enter order id to deliver\n");
            scanf("%d", &n);
            delivery(n);
            break;
        case 6:
            printf("Enter order id to cancel\n");
            scanf("%d", &n);
            cancel(n);
            break;
        case 7:
            printf("Enter the user id to search favourite food:\n");
            scanf("%d", &n);
            find_favourite_foods_of_user(n);
            break;
        case 8:
            printf("Enter the no of days:\n");
            scanf("%d", &n);
            find_favourite_restaurants(n);
            break;
        case 9:
            printf("Enter the no of days:\n");
            scanf("%d", &n);
            find_favourite_food_across_restaurants(n);
            break;
        case 10:
            print_agent_database(agent_root);
            break;
        case 11:
            print_live_orders(order_root);
            break;
        case 12:
            print_area_wise_agents(agent_root, "area1");
            break;
        case 13:
        
            print_res_details(res_root, "Restaurant2", "Address2");
            break;
        case 14:
            printf("Enter the id1 and id2 for range:\n");
            scanf("%d %d", &id1, &id2);
            range_search(user_root, id1, id2);
            break;
        
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice >=0);

    return 0;
}