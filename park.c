#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "park.h"

enum { 
    EIDPARK,
    EIDCLI,
    EETAG,
    ETMPSRES,
    EDISP,
    EAGENT,  // Nouvelle colonne pour l'agent affecté
    COLUMNS
};

// Définition de la structure parking
typedef struct {
    char idpark[30];
    char idclient[10];
    int etage;
    char tempsres[10];
    char disponibility[30];
    char agent[10]; // Champ pour l'agent affecté ("Oui" ou "Non")
} parking;

// Fonction Ajouter
void ajouter_park(parking b, char *filename) {
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%s %s %d %s %s %s\n", b.idpark, b.idclient, b.etage, b.tempsres, b.disponibility, b.agent);
        fclose(f);
    }
}

// Fonction Afficher
void afficher_park(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    char idpark[30], idclient[10], etage[10], tempsres[10], disponibility[30], agent[10];
    store = gtk_tree_view_get_model(liste);

    if (store == NULL) {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Parking", renderer, "text", EIDPARK, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Client", renderer, "text", EIDCLI, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Etage", renderer, "text", EETAG, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Temps Reservation", renderer, "text", ETMPSRES, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Disponibilité", renderer, "text", EDISP, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Agent Affecté", renderer, "text", EAGENT, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    FILE *f = fopen("parking.txt", "r");

    if (f != NULL) {
        while (fscanf(f, "%s %s %s %s %s %s\n", idpark, idclient, etage, tempsres, disponibility, agent) != EOF) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, EIDPARK, idpark, EIDCLI, idclient, EETAG, etage, ETMPSRES, tempsres, EDISP, disponibility, EAGENT, agent, -1);
        }
        fclose(f);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

// Fonction Modifier
void modifier_park(char idpark[], parking b1, char *filename) {
    parking b;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %d %s %s %s\n", b.idpark, b.idclient, &b.etage, b.tempsres, b.disponibility, b.agent) != EOF) {
            if (strcmp(b.idpark, idpark) == 0)
                fprintf(f2, "%s %s %d %s %s %s\n", b1.idpark, b1.idclient, b1.etage, b1.tempsres, b1.disponibility, b1.agent);
            else
                fprintf(f2, "%s %s %d %s %s %s\n", b.idpark, b.idclient, b.etage, b.tempsres, b.disponibility, b.agent);
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    }
}

// Fonction Supprimer
void supprimer_park(char idpark[], char *filename) {
    parking b;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%s %s %d %s %s %s\n", b.idpark, b.idclient, &b.etage, b.tempsres, b.disponibility, b.agent) != EOF) {
            if (strcmp(b.idpark, idpark) != 0)
                fprintf(f2, "%s %s %d %s %s %s\n", b.idpark, b.idclient, b.etage, b.tempsres, b.disponibility, b.agent);
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    }
}
