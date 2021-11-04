/* vim: set sw=4 sts=4 ft=cpp et foldmethod=syntax : */

/*
 * Copyright (c) 2011 Tiziano Müller <tm@dev-zero.ch>
 *
 *
 *
 */

#ifndef PARTICLE_HH
#define PARTICLE_HH

struct Particle
{
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
    double m;
    double rho;
    double T;
    // Save the material information of a particle
    int mat;
};

#endif // PARTICLE_HH
