// park.h
#ifndef PARK_H
#define PARK_H

typedef struct {
    char idpark[20];
    char idclient[20];
    int etage;
    char temps_reservation[50];  // Temps de réservation
    char disponibility[20];      // Disponibilité
    char agent[20];              // Agent
} parking;

#endif
