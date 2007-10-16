
/*
 * File: odometer.h
 * Classe: Odometer
 * Autore: Emanuele Aina <em@nerd.ocracy.org>
 * Licenza: LGPL
 * Note: implements an absolute encoder from a relative one
 */

#ifndef ODOMETER_H
#define ODOMETER_H

enum Direction {
  DIRECTION_NONE,
  DIRECTION_COUNTERCLOCKWISE,
  DIRECTION_CLOCKWISE
};

typedef struct {
    int coeff;
    int K, K1, K2;
    int angle;
    int revolutions;

    int started;
    enum Direction direction;
} Odometer;

void odometer_init();
void odometer_set_coeff(int command_data);
void odometer_set_K(int command_data);
void odometer_set_K1(int command_data);
void odometer_set_K2(int command_data);
void odometer_get_angle(int command_data);
void odometer_get_revolutions(int command_data);
void odometer_rotation_counterclockwise();
void odometer_rotation_clockwise();
void odometer_revolution();

#endif /* ODOMETER_H */
