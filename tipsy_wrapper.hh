#ifndef TIPSY_WRAPPER_HH
#define TIPSY_WRAPPER_HH

extern "C" {
#include "tipsy.h"
}

#include <string>
#include <cassert>

#include "particle.hh"

class TipsyInFile
{
private:
    TCTX ctx;
    int n, N;
    double t;

public:
    TipsyInFile() :
        ctx(0), n(0), N(0), t(0.0)
    {
    }

    TipsyInFile(const std::string& f) :
        ctx(0), n(0), N(0), t(0.0)
    {
        open(f);
    }

    void open(const std::string& f)
    {
        if (ctx)
            close();

        // casting away const is ugly,  but the Tipsy API should be corrected
        TipsyInitialize(&ctx, 0, const_cast<char*>(f.c_str()));

        // Read number of particles and simulation time
        N = iTipsyNumParticles(ctx);
        n = 0;
        t = dTipsyTime(ctx);
    }

    ~TipsyInFile()
    {
        close();
    }

    void close()
    {
        if (ctx)
        {
            TipsyFinish(ctx);
            ctx = 0;
        }
    }

    bool eof() const
    {
        assert(ctx != 0 && "file not open");
        return (n >= N);
    }

    size_t size() const
    {
        assert(ctx != 0 && "file not open");
        return N;
    }

    double time() const
    {
        assert(ctx != 0 && "file not open");
        return t;
    }

    void operator>>(Particle& particle)
    {
        assert(ctx != 0 && "file not open");

        int type;
        double dSoft;

        // Read one particle from the input file
        base_particle *p = pTipsyRead(ctx, &type, &dSoft);

        // Make sure it is a gas particle
        assert(type == TIPSY_TYPE_GAS);

        gas_particle *g = (gas_particle *)p;

        particle.m = g->mass;

        particle.x = g->pos[0];
        particle.y = g->pos[1];
        particle.z = g->pos[2];

        particle.vx = g->vel[0];
        particle.vy = g->vel[1];
        particle.vz = g->vel[2];

        particle.rho = g->rho;
        particle.T = g->temp;
		
		// Save material information in metals
        particle.mat = (int) g->metals;
        n = n + 1;
    }
};

class TipsyOutFile
{
private:
    TCTX ctx;
    std::string file;
    double t;

public:

    TipsyOutFile() :
        ctx(0), file(""), t(0)
    {
    }

    TipsyOutFile(const std::string& f) :
        ctx(0), file(f), t(0)
    {
        open(f);
    }

    void open(const std::string& f)
    {
        if (ctx)
            close();

        file = f;
        TipsyInitialize(&ctx, 0, NULL);
    }

    ~TipsyOutFile()
    {
        close();
    }

    void close() {
        if (ctx)
        {
            TipsyWriteAll(ctx, t, const_cast<char*>(file.c_str()));
            TipsyFinish(ctx);
            ctx = 0;
        }
    }

    void time(double time)
    {
        assert(time >= 0);
        t = time;
    }

    void operator<<(const Particle& particle)
    {
        assert(ctx != NULL && "file not open");

        gas_particle g;

        g.pos[0] = particle.x;
        g.pos[1] = particle.y;
        g.pos[2] = particle.z;

        g.vel[0] = particle.vx;
        g.vel[1] = particle.vy;
        g.vel[2] = particle.vz;

        g.mass = particle.m;
        g.rho  = particle.rho;
        g.temp = particle.T;

        g.hsmooth = 0.1;
        g.metals = (float) particle.mat;
        g.phi = 0;

        // Add gas particle to Tipsy Structure
        TipsyAddGas(ctx, &g);
    }
};

#endif // TIPSY_WRAPPER_HH
