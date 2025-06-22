#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

GtkWidget *entry;
char current_input[256] = "";

void update_entry() {
    gtk_entry_set_text(GTK_ENTRY(entry), current_input);
}

void on_number_click(GtkWidget *widget, gpointer data) {
    const char *num = (const char *)data;
    strcat(current_input, num);
    update_entry();
}

void on_clear_click(GtkWidget *widget, gpointer data) {
    current_input[0] = '\0';
    update_entry();
}

void on_operator_click(GtkWidget *widget, gpointer data) {
    const char *op = (const char *)data;
    if (strlen(current_input) == 0) return;

    int len = strlen(current_input);
    if (!strchr("+-*/", current_input[len - 1])) {
        strcat(current_input, op);
        update_entry();
    }
}

void on_equals_click(GtkWidget *widget, gpointer data) {
    char op = 0;
    int pos = -1;
    for (int i = 1; current_input[i]; i++) {
        if (strchr("+-*/", current_input[i])) {
            op = current_input[i];
            pos = i;
            break;
        }
    }

    if (op == 0 || pos == -1) return;

    char left[128] = "", right[128] = "";
    strncpy(left, current_input, pos);
    left[pos] = '\0';
    strcpy(right, &current_input[pos + 1]);

    double a = atof(left);
    double b = atof(right);
    double result = 0;

    if (op == '+') result = a + b;
    else if (op == '-') result = a - b;
    else if (op == '*') result = a * b;
    else if (op == '/') {
        if (b == 0) {
            strcpy(current_input, "Error");
            update_entry();
            return;
        }
        result = a / b;
    }

    snprintf(current_input, sizeof(current_input), "%g", result);
    update_entry();
}

GtkWidget* create_button(const char *label, GCallback callback, gpointer data) {
    GtkWidget *button = gtk_button_new_with_label(label);
    g_signal_connect(button, "clicked", callback, data);
    return button;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "계산기");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    const char *buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "=", "+"}
    };

    for (int i = 0; i < 4; i++) {
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

        for (int j = 0; j < 4; j++) {
            const char *label = buttons[i][j];
            GtkWidget *button;

            if (strcmp(label, "C") == 0) {
                button = create_button(label, G_CALLBACK(on_clear_click), NULL);
            } else if (strcmp(label, "=") == 0) {
                button = create_button(label, G_CALLBACK(on_equals_click), NULL);
            } else if (strchr("+-*/", label[0]) != NULL) {
                button = create_button(label, G_CALLBACK(on_operator_click), (gpointer)label);
            } else {
                button = create_button(label, G_CALLBACK(on_number_click), (gpointer)label);
            }

            gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
        }
    }

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
