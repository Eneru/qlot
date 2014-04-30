#include <iostream>
#include <cstring>
#include "mainwindow.h"
#include "ui_mainwindow.h"

char * int_to_couleur(int coul)
{
    char * list [23] = {"Cyan","Magenta","Emeraude","Jaune","Bleu","Rouge","Vert","Saphir","Kaki","Brun","Marron","Rose","Violet","Mauve","Pourpre","Bleu roi","Ocre","Noir","Blanc","Gris","Beige","Creme","Fluo"};
    return list[coul];
}

int couleur_to_int (const char * c)
{
    char * list [23] = {"Cyan","Magenta","Emeraude","Jaune","Bleu","Rouge","Vert","Saphir","Kaki","Brun","Marron","Rose","Violet","Mauve","Pourpre","Bleu roi","Ocre","Noir","Blanc","Gris","Beige","Creme","Fluo"};
    for (int i = 0 ; i<22 ; i++)
        if (strcmp (c, list[i]) == 0)
            return i;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Action menu
    QObject::connect(ui->actionVersion,SIGNAL(triggered()),this,SLOT(version()),Qt::AutoConnection);
    QObject::connect(ui->actionAjouter_une_marchandise,SIGNAL(triggered()),this,SLOT(ajoutMarchandise()),Qt::AutoConnection);
    QObject::connect(ui->actionAjouter_une_vente,SIGNAL(triggered()),this,SLOT(ajoutVente()),Qt::AutoConnection);
    QObject::connect(ui->actionNouvelle_BDD,SIGNAL(triggered()),this,SLOT(nouvellebdd()),Qt::AutoConnection);
    QObject::connect(ui->actionEnregistrer,SIGNAL(triggered()),this,SLOT(enregistrer()),Qt::AutoConnection);
    QObject::connect(ui->actionOuvrir_une_BDD,SIGNAL(triggered()),this,SLOT(ouvrir()),Qt::AutoConnection);
    QObject::connect(ui->actionEconomies,SIGNAL(triggered()),this,SLOT(statistiques()),Qt::AutoConnection);
    QObject::connect(ui->actionAnnuler_la_vente,SIGNAL(triggered()),this,SLOT(annuler_vente()),Qt::AutoConnection);
    QObject::connect(ui->actionStock_total,SIGNAL(triggered()),this,SLOT(stock_total()),Qt::AutoConnection);

    // Gestion
    QObject::connect(ui->critere_combo_box_gestion,SIGNAL(currentIndexChanged(int)),this,SLOT(changement_affichage_gestion()),Qt::AutoConnection);
    QObject::connect(ui->doubleSpinBox_gestion,SIGNAL(valueChanged(double)),this,SLOT(changement_affichage_gestion()),Qt::AutoConnection);
    QObject::connect(ui->doubleSpinBox_prix_max,SIGNAL(valueChanged(double)),this,SLOT(changement_affichage_gestion()),Qt::AutoConnection);
    QObject::connect(ui->tableau_recherche_gestion,SIGNAL(itemSelectionChanged()),this,SLOT(enablement_modif()),Qt::AutoConnection);
    QObject::connect(ui->modif_article,SIGNAL(clicked()),this,SLOT(modif_article()),Qt::AutoConnection);

    // Vente
    QObject::connect(ui->critere_combo_box_vente,SIGNAL(currentIndexChanged(int)),this,SLOT(changement_affichage_vente()),Qt::AutoConnection);
    QObject::connect(ui->tableWidget_vente,SIGNAL(itemSelectionChanged()),this,SLOT(enablement_detail_vente()),Qt::AutoConnection);
    QObject::connect(ui->boutton_detail_vente,SIGNAL(clicked()),this,SLOT(ouvre_detail()),Qt::AutoConnection);
    QObject::connect(ui->doubleSpinBox_vente,SIGNAL(valueChanged(double)),this,SLOT(changement_affichage_vente()),Qt::AutoConnection);
    QObject::connect(ui->doubleSpinBox_prix_max_vente,SIGNAL(valueChanged(double)),this,SLOT(changement_affichage_vente()),Qt::AutoConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stock_total()
{
    StockDialog sckd(this);
    sckd.exec();
}

void MainWindow::annuler_vente()
{
    m.annuler_vente(m.nombre_ventes()-1);

    affichage_gestion();
    affichage_vente();
}

void MainWindow::statistiques()
{
    StatDialog sd(this);
    sd.exec();
}

void MainWindow::version()
{
    VersionDialog vd(this);
    vd.exec();
}

void MainWindow::ajoutMarchandise()
{
    AjoutMarchandiseDialog amd(this);
    amd.exec();

    affichage_gestion();
}

void MainWindow::ajoutVente()
{
    AjoutVenteDialog avd(this);
    avd.exec();

    affichage_gestion();
    affichage_vente();
}

void MainWindow::nouvellebdd()
{
    magasin mprime;
    m.swap(mprime);
    ui->tableau_recherche_gestion->setRowCount(0);
    ui->tableWidget_vente->setRowCount(0);
}

void MainWindow::enregistrer()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this,tr("Enregistrer un fichier de gestion QloT"),QString(),tr("FichierQloT *.qlot(*.qlot *.culotte)"));
    es::sauvegarder_magasin(filename.toStdString(),m);
}

void MainWindow::ouvrir()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,tr("Ouvrir un fichier de gestion QloT"),QString(),tr("FichierQloT *.qlot(*.qlot *.culotte)"));
    es::charger_magasin(filename.toStdString(),m);
    std::cout << filename.toStdString() << std::endl;
    affichage_gestion();
    affichage_vente();
}

void MainWindow::affichage_gestion()
{
    if (m.nombre_articles()>0)
    {
        if (ui->critere_combo_box_gestion->currentIndex() == 0)
        {
            char mod[m.nombre_articles()][4];
            char tail[m.nombre_articles()][2];
            char quant[m.nombre_articles()][2];
            char dat[m.nombre_articles()][12];
            char ach[m.nombre_articles()][8];
            char vent[m.nombre_articles()][8];
            char rab[m.nombre_articles()][8];

            ui->tableau_recherche_gestion->setRowCount(m.nombre_articles());

            QTableWidgetItem * tab[m.nombre_articles()][8];
            std::vector<article_affichage> vacla = m.articles();

            for(int i = 0 ; i<m.nombre_articles() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }

        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 1) // recherche par modele
        {
            std::vector<article_affichage> vacla = m.articles_par_modele((unsigned int)ui->doubleSpinBox_gestion->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 2) // recherche par couleur
        {
            std::vector<article_affichage> vacla = m.articles_par_couleur((unsigned int)ui->doubleSpinBox_gestion->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 3) // recherche par taille
        {
            std::vector<article_affichage> vacla = m.articles_par_taille((unsigned int)ui->doubleSpinBox_gestion->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 4) // recherche par achat
        {
            std::vector<article_affichage> vacla = m.articles_par_prix_achat(ui->doubleSpinBox_gestion->value(),ui->doubleSpinBox_prix_max->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 5) // recherche par vente
        {
            std::vector<article_affichage> vacla = m.articles_par_prix_achat(ui->doubleSpinBox_gestion->value(),ui->doubleSpinBox_prix_max->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }

        else if (ui->critere_combo_box_gestion->currentIndex() == 6) // recherche par prix effectif
        {
            std::vector<article_affichage> vacla = m.articles_par_prix_achat(ui->doubleSpinBox_gestion->value(),ui->doubleSpinBox_prix_max->value());

            char mod[vacla.size()][4];
            char tail[vacla.size()][2];
            char quant[vacla.size()][2];
            char dat[vacla.size()][12];
            char ach[vacla.size()][8];
            char vent[vacla.size()][8];
            char rab[vacla.size()][8];

            ui->tableau_recherche_gestion->setRowCount(vacla.size());

            QTableWidgetItem * tab[vacla.size()][8];

            for(int i = 0 ; i<vacla.size() ; i++)
            {
                article_affichage i_eme_article = vacla[i];

                for (int j = 0 ; j<8 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableau_recherche_gestion->setItem(i,j,tab[i][j]);
                }

                sprintf(mod[i],"%u",i_eme_article.reference().modele());
                ui->tableau_recherche_gestion->item(i,0)->setText(mod[i]);

                ui->tableau_recherche_gestion->item(i,1)->setText(int_to_couleur(i_eme_article.reference().couleur()));

                sprintf(tail[i],"%u",i_eme_article.reference().taille());
                ui->tableau_recherche_gestion->item(i,2)->setText(tail[i]);

                sprintf(quant[i],"%u",i_eme_article.quantite_stock());
                ui->tableau_recherche_gestion->item(i,3)->setText(quant[i]);

                sprintf(dat[i],"%d/%d/%d",i_eme_article.date_livraison().jour(),i_eme_article.date_livraison().mois(),i_eme_article.date_livraison().annee());
                ui->tableau_recherche_gestion->item(i,4)->setText(dat[i]);

                sprintf(ach[i],"%.2f euros",i_eme_article.prix_achat());
                ui->tableau_recherche_gestion->item(i,5)->setText(ach[i]);

                sprintf(vent[i],"%.2f euros",i_eme_article.prix_vente());
                ui->tableau_recherche_gestion->item(i,6)->setText(vent[i]);

                sprintf(rab[i],"%.2f euros",i_eme_article.prix_vente_effectif());
                ui->tableau_recherche_gestion->item(i,7)->setText(rab[i]);
            }
        }
    }

    else
        ui->tableau_recherche_gestion->setRowCount(0);
}

void MainWindow::changement_affichage_gestion()
{
    if(ui->critere_combo_box_gestion->currentIndex() != 0)
    {
        ui->doubleSpinBox_gestion->setEnabled(true);

        switch(ui->critere_combo_box_gestion->currentIndex())
        {
            case 1 : ui->doubleSpinBox_gestion->setMinimum(1); // modele
                    ui->doubleSpinBox_gestion->setMaximum(9999);
                    ui->doubleSpinBox_gestion->setDecimals(0);
                    ui->doubleSpinBox_gestion->setSingleStep(1.);
                    ui->doubleSpinBox_gestion->setSuffix(tr(""));
                    ui->doubleSpinBox_prix_max->setEnabled(false);
                    break;

            case 2 : ui->doubleSpinBox_gestion->setMinimum(0); // couleur
                    ui->doubleSpinBox_gestion->setMaximum(22);
                    ui->doubleSpinBox_gestion->setDecimals(0);
                    ui->doubleSpinBox_gestion->setSingleStep(1.);
                    char buff [256];
                    sprintf(buff," (%s)",int_to_couleur((int)ui->doubleSpinBox_gestion->value()));
                    ui->doubleSpinBox_gestion->setSuffix(tr(buff));
                    ui->doubleSpinBox_prix_max->setEnabled(false);
                    break;

            case 3 : ui->doubleSpinBox_gestion->setMinimum(15); // taille
                    ui->doubleSpinBox_gestion->setMaximum(50);
                    ui->doubleSpinBox_gestion->setDecimals(0);
                    ui->doubleSpinBox_gestion->setSingleStep(1.);
                    ui->doubleSpinBox_gestion->setSuffix(tr(" (EU)"));
                    ui->doubleSpinBox_prix_max->setEnabled(false);
                    break;

            case 4 : // achat

            case 5 : // vente

            case 6 : ui->doubleSpinBox_gestion->setMinimum(0.99); // vente effective
                    ui->doubleSpinBox_gestion->setMaximum(499.99);
                    ui->doubleSpinBox_gestion->setDecimals(2);
                    ui->doubleSpinBox_gestion->setSingleStep(0.01);
                    ui->doubleSpinBox_gestion->setSuffix(tr(" euros"));
                    ui->doubleSpinBox_prix_max->setEnabled(true);
                    if(ui->doubleSpinBox_gestion->value()>=ui->doubleSpinBox_prix_max->value()) // si le prix max est en dessous -> automatiquement 0.01 au dessus
                        ui->doubleSpinBox_prix_max->setValue(ui->doubleSpinBox_gestion->value()+0.01);
                    break;
        }
    }
    else
    {
        ui->doubleSpinBox_gestion->setEnabled(false);
        ui->doubleSpinBox_prix_max->setEnabled(false);
    }

    affichage_gestion();
}

void MainWindow::changement_affichage_vente()
{
    if(ui->critere_combo_box_vente->currentIndex() > 0)
    {
        ui->doubleSpinBox_vente->setEnabled(true);

        switch(ui->critere_combo_box_vente->currentIndex())
        {
            case 1 : ui->doubleSpinBox_vente->setMinimum(0); // identifiant
                    ui->doubleSpinBox_vente->setMaximum(m.nombre_ventes()-1);
                    ui->doubleSpinBox_vente->setDecimals(0);
                    ui->doubleSpinBox_vente->setSingleStep(1.);
                    ui->doubleSpinBox_vente->setSuffix(tr(""));
                    ui->doubleSpinBox_prix_max_vente->setEnabled(false);
                    break;

            case 2 : ui->doubleSpinBox_vente->setMinimum(0); // quantite articles
                    ui->doubleSpinBox_vente->setMaximum(9999);
                    ui->doubleSpinBox_vente->setDecimals(0);
                    ui->doubleSpinBox_vente->setSingleStep(1.);
                    ui->doubleSpinBox_vente->setSuffix(tr(""));
                    ui->doubleSpinBox_prix_max_vente->setEnabled(false);
                    break;

            case 3 : ui->doubleSpinBox_vente->setMinimum(0.99); // prix total
                    ui->doubleSpinBox_vente->setMaximum(99999999.99);
                    ui->doubleSpinBox_vente->setDecimals(2);
                    ui->doubleSpinBox_vente->setSingleStep(0.01);
                    ui->doubleSpinBox_vente->setSuffix(tr(" euros"));
                    ui->doubleSpinBox_prix_max_vente->setEnabled(true);
                    if(ui->doubleSpinBox_vente->value()>=ui->doubleSpinBox_prix_max_vente->value()) // si le prix max est en dessous -> automatiquement 0.01 au dessus
                        ui->doubleSpinBox_prix_max_vente->setValue(ui->doubleSpinBox_vente->value()+0.01);
                    break;
        }
    }
    else
    {
        ui->doubleSpinBox_vente->setEnabled(false);
        ui->doubleSpinBox_prix_max_vente->setEnabled(false);
    }

    affichage_vente();
}

void MainWindow::affichage_vente()
{
    if (m.nombre_ventes()>0)
    {
        if (ui->critere_combo_box_vente->currentIndex() == 0)
        {
            int nombre_vente = m.nombre_ventes();
            ui->tableWidget_vente->setRowCount(nombre_vente);

            QTableWidgetItem * tab[nombre_vente][4];
            char id[nombre_vente][10];
            char dat[nombre_vente][11];
            char qte[nombre_vente][5];
            char pr_t[nombre_vente][8];

            for (int i = 0 ; i<nombre_vente ; i++)
            {
                for (int j = 0 ; j<4 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableWidget_vente->setItem(i,j,tab[i][j]);
                }
                std::vector<vente> vv = m.ventes();

                std::cout << vv[i] << std::endl;
                sprintf(id[i],"%u",vv[i].id());
                ui->tableWidget_vente->item(i,0)->setText(id[i]);

                sprintf(dat[i],"%d/%d/%d",vv[i].date_vente().jour(),vv[i].date_vente().mois(),vv[i].date_vente().annee());
                ui->tableWidget_vente->item(i,1)->setText(dat[i]);

                sprintf(qte[i],"%u",vv[i].quantite_articles_vendus());
                ui->tableWidget_vente->item(i,2)->setText(qte[i]);

                sprintf(pr_t[i],"%.2f",vv[i].total());
                ui->tableWidget_vente->item(i,3)->setText(pr_t[i]);
            }
        }
        else if (ui->critere_combo_box_vente->currentIndex() == 1) // recherche par identifiant (en gros date)
        {
            int selec = (int)ui->doubleSpinBox_vente->value();
            ui->tableWidget_vente->setRowCount(1);

            QTableWidgetItem * tab[4];

            char id[10];
            char dat[11];
            char qte[5];
            char pr_t[8];

            for (int j = 0 ; j<4 ; j++)
            {
                tab[j] = new QTableWidgetItem;
                ui->tableWidget_vente->setItem(j,0,tab[j]);
            }

            std::vector<vente> vv = m.ventes();

            sprintf(id,"%u",vv[selec].id());
            ui->tableWidget_vente->item(0,0)->setText(id);

            sprintf(dat,"%d/%d/%d",vv[selec].date_vente().jour(),vv[selec].date_vente().mois(),vv[selec].date_vente().annee());
            ui->tableWidget_vente->item(0,1)->setText(dat);

            sprintf(qte,"%u",vv[selec].quantite_articles_vendus());
            ui->tableWidget_vente->item(0,2)->setText(qte);

            sprintf(pr_t,"%.2f",vv[selec].total());
            ui->tableWidget_vente->item(0,3)->setText(pr_t);
        }
        else if (ui->critere_combo_box_vente->currentIndex() == 2)
        {
            int selec = (int)ui->doubleSpinBox_vente->value();

            std::vector<vente> vv = m.ventes_par_quantite(selec);

            unsigned int nombre_vente = vv.size();

            ui->tableWidget_vente->setRowCount(nombre_vente);

            QTableWidgetItem * tab[nombre_vente][4];
            char id[nombre_vente][10];
            char dat[nombre_vente][11];
            char qte[nombre_vente][5];
            char pr_t[nombre_vente][8];

            for (int i = 0 ; i<nombre_vente ; i++)
            {
                for (int j = 0 ; j<4 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableWidget_vente->setItem(i,j,tab[i][j]);
                }


                std::cout << vv[i] << std::endl;
                sprintf(id[i],"%u",vv[i].id());
                ui->tableWidget_vente->item(i,0)->setText(id[i]);

                sprintf(dat[i],"%d/%d/%d",vv[i].date_vente().jour(),vv[i].date_vente().mois(),vv[i].date_vente().annee());
                ui->tableWidget_vente->item(i,1)->setText(dat[i]);

                sprintf(qte[i],"%u",vv[i].quantite_articles_vendus());
                ui->tableWidget_vente->item(i,2)->setText(qte[i]);

                sprintf(pr_t[i],"%.2f",vv[i].total());
                ui->tableWidget_vente->item(i,3)->setText(pr_t[i]);
            }
        }
        else if (ui->critere_combo_box_vente->currentIndex() == 3)
        {
            double selec1 = ui->doubleSpinBox_vente->value();
            double selec2 = ui->doubleSpinBox_prix_max_vente->value();

            std::vector<vente> vv = m.ventes_par_prix_total(selec1,selec2);

            unsigned int nombre_vente = vv.size();

            ui->tableWidget_vente->setRowCount(nombre_vente);

            QTableWidgetItem * tab[nombre_vente][4];
            char id[nombre_vente][10];
            char dat[nombre_vente][11];
            char qte[nombre_vente][5];
            char pr_t[nombre_vente][8];

            for (int i = 0 ; i<nombre_vente ; i++)
            {
                for (int j = 0 ; j<4 ; j++)
                {
                    tab[i][j] = new QTableWidgetItem;
                    ui->tableWidget_vente->setItem(i,j,tab[i][j]);
                }


                std::cout << vv[i] << std::endl;
                sprintf(id[i],"%u",vv[i].id());
                ui->tableWidget_vente->item(i,0)->setText(id[i]);

                sprintf(dat[i],"%d/%d/%d",vv[i].date_vente().jour(),vv[i].date_vente().mois(),vv[i].date_vente().annee());
                ui->tableWidget_vente->item(i,1)->setText(dat[i]);

                sprintf(qte[i],"%u",vv[i].quantite_articles_vendus());
                ui->tableWidget_vente->item(i,2)->setText(qte[i]);

                sprintf(pr_t[i],"%.2f",vv[i].total());
                ui->tableWidget_vente->item(i,3)->setText(pr_t[i]);
            }
        }
    }
    else
        ui->tableWidget_vente->setRowCount(0);
}

void MainWindow::enablement_detail_vente()
{
    if (ui->tableWidget_vente->rowCount()>0)
        ui->boutton_detail_vente->setEnabled(true);
    else ui->boutton_detail_vente->setEnabled(false);
}

void MainWindow::enablement_modif()
{
    if (ui->tableau_recherche_gestion->rowCount()>0)
        ui->modif_article->setEnabled(true);
    else ui->modif_article->setEnabled(false);
}

void MainWindow::ouvre_detail()
{
    id_vente = ui->tableWidget_vente->item(ui->tableWidget_vente->currentRow(),0)->text().toInt();
    DetailVenteDialog dvd(this);
    dvd.exec();
}

void MainWindow::modif_article()
{
    unsigned int modele = ui->tableau_recherche_gestion->item(ui->tableau_recherche_gestion->currentRow(),0)->text().toInt();
    unsigned int couleur = couleur_to_int(ui->tableau_recherche_gestion->item(ui->tableau_recherche_gestion->currentRow(),1)->text().toStdString().c_str());
    unsigned int taille = ui->tableau_recherche_gestion->item(ui->tableau_recherche_gestion->currentRow(),2)->text().toInt();
    reference_article rfa (modele,couleur,taille);
    id_vente = rfa.vers_entier();
    ModifArticleDialog mad(this);
    mad.exec();

    affichage_gestion();
}
