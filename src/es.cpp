/**
 * \file es.cpp
 * \brief Entrées / Sorties.
 * \author RAZANAJATO RANAIVOARIVONY Harenome
 * \author MEYER Jérémy
 * \date 2014
 * \copyright WTFPL version 2
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */
#include "es.hpp"

bool es::charger_magasin (std::string chemin, magasin & m)
{
    return es::charger_magasin (chemin.c_str (), m);
}

bool es::charger_magasin (const char * chemin, magasin & m)
{
    std::ifstream fichier (chemin, std::ifstream::in);
    es::charger_magasin (fichier, m);
    fichier.close ();

    return ! fichier.fail ();
}

bool es::charger_magasin (std::ifstream & fichier, magasin & m)
{
    fichier >> m;
    return ! fichier.fail ();
}

bool es::sauvegarder_magasin (std::string chemin, const magasin & m)
{
    return es::sauvegarder_magasin (chemin.c_str (), m);
}

bool es::sauvegarder_magasin (const char * chemin, const magasin & m)
{
    std::ofstream fichier (chemin, std::ofstream::out);
    es::sauvegarder_magasin (fichier, m);
    fichier.close ();

    return ! fichier.fail ();
}

bool es::sauvegarder_magasin (std::ofstream & fichier, const magasin & m)
{
    fichier << m;
    return ! fichier.fail ();
}
