#include "guictrl.h"
#include <iostream>
using namespace std;

GuiCtrl::GuiCtrl(){
    GlWidget view;
}

void GuiCtrl::connectGUI(){
    QObject::connect(dial_zoom, SIGNAL(valueChanged(int)), this, SLOT(zoom()));
    QObject::connect(horizontalSlider_x, SIGNAL(valueChanged(int)), this, SLOT(rotateX()));
    QObject::connect(horizontalSlider_y, SIGNAL(valueChanged(int)), this, SLOT(rotateY()));
    QObject::connect(horizontalSlider_z, SIGNAL(valueChanged(int)), this, SLOT(rotateZ()));
    QObject::connect(pushButton_reset_rotation, SIGNAL(clicked(bool)), this, SLOT(resetRotation()));
    QObject::connect(radioButton_hide, SIGNAL(toggled(bool)), this, SLOT(hide()));
    QObject::connect(radioButton_realcolor, SIGNAL(toggled(bool)), this, SLOT(real()));
    QObject::connect(radioButton_normal, SIGNAL(toggled(bool)), this, SLOT(normal()));

    QObject::connect(comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(parts()));
    QObject::connect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));

    QObject::connect(doubleSpinBox_ax, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::connect(doubleSpinBox_ay, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::connect(doubleSpinBox_az, SIGNAL(valueChanged(double)), this, SLOT(alterA()));

    QObject::connect(doubleSpinBox_bx, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::connect(doubleSpinBox_by, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::connect(doubleSpinBox_bz, SIGNAL(valueChanged(double)), this, SLOT(alterB()));

    QObject::connect(doubleSpinBox_cx, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::connect(doubleSpinBox_cy, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::connect(doubleSpinBox_cz, SIGNAL(valueChanged(double)), this, SLOT(alterC()));

    QObject::connect(doubleSpinBox_dx, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::connect(doubleSpinBox_dy, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::connect(doubleSpinBox_dz, SIGNAL(valueChanged(double)), this, SLOT(alterD()));

    QObject::connect(pushButton_new, SIGNAL(pressed()), this, SLOT(newPart()));
    QObject::connect(pushButton_save, SIGNAL(pressed()), this, SLOT(savePart()));
    QObject::connect(pushButton_del, SIGNAL(pressed()), this, SLOT(deletePart()));
    QObject::connect(pushButton_name, SIGNAL(pressed()), this, SLOT(newName()));
    QObject::connect(pushButton_delName, SIGNAL(pressed()), this, SLOT(delName()));

    QObject::connect(pushButton_color, SIGNAL(pressed()), this, SLOT(colorDialog()));

    QObject::connect(pushButton_normal, SIGNAL(pressed()), this, SLOT(setNormal()));
}

void GuiCtrl::zoom(){
    switch(dial_zoom->value()){
    case 0:
        view.resizeGL(WIDTH,HEIGHT);
        view.paintGL(0.25, rotation, comboBox_part->currentText());
        break;
    case 1:
        view.resizeGL(WIDTH,HEIGHT);
        view.paintGL(0.5, rotation, comboBox_part->currentText());
        break;
    case 2:
        view.resizeGL(WIDTH,HEIGHT);
        view.paintGL(1.0, rotation, comboBox_part->currentText());
        break;
    }
}

void GuiCtrl::rotateX(){
    rotation[0] = horizontalSlider_x->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::rotateY(){
    rotation[1] = horizontalSlider_y->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::rotateZ(){
    rotation[2] = horizontalSlider_z->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::resetRotation(){
    horizontalSlider_x->setValue(0);
    horizontalSlider_y->setValue(0);
    horizontalSlider_z->setValue(0);
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::addNames(){
    comboBox_name->clear();
    for(int i=0; i<(int)Db::names.size(); i++){
        comboBox_name->addItem(QString::number(Db::names[i]));
    }
}

void GuiCtrl::hide(){
    display == 0 ? display = 1 : display = 0;
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::real(){
    REAL == 0 ? REAL = 1 : REAL = 0;
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::normal(){
    showNormals ? showNormals = 0 : showNormals = 1;
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::parts(){
    QObject::disconnect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
    comboBox_part->setCurrentIndex(0);
    QObject::connect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
    selectParts(comboBox_name->currentText());
    addParts();
}

void GuiCtrl::addParts(){
    comboBox_part->clear();
    for(int i=0; i<(int)Db::parts.size(); i++){
        comboBox_part->addItem(QString::number(Db::parts[i]));
    }
}

void GuiCtrl::choosePart(){
    modPart = comboBox_part->currentText().toInt();
    selectConstruct(comboBox_name->currentText());
    initA(comboBox_part->currentText());
    initB(comboBox_part->currentText());
    initC(comboBox_part->currentText());
    initD(comboBox_part->currentText());
    selectNormal(comboBox_name->currentText());
    view.paintGL(1.0, rotation, comboBox_part->currentText());
    textBrowser->append(selPart);
}

void GuiCtrl::initA(QString part){
    int n = part.toInt();
    QObject::disconnect(doubleSpinBox_ax, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::disconnect(doubleSpinBox_ay, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::disconnect(doubleSpinBox_az, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    doubleSpinBox_ax->setValue(Db::construct[n][0][0]);
    doubleSpinBox_ay->setValue(Db::construct[n][0][1]);
    doubleSpinBox_az->setValue(Db::construct[n][0][2]);
    QObject::connect(doubleSpinBox_ax, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::connect(doubleSpinBox_ay, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
    QObject::connect(doubleSpinBox_az, SIGNAL(valueChanged(double)), this, SLOT(alterA()));
}

void GuiCtrl::initB(QString part){
    int n = part.toInt();
    QObject::disconnect(doubleSpinBox_bx, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::disconnect(doubleSpinBox_by, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::disconnect(doubleSpinBox_bz, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    doubleSpinBox_bx->setValue(Db::construct[n][1][0]);
    doubleSpinBox_by->setValue(Db::construct[n][1][1]);
    doubleSpinBox_bz->setValue(Db::construct[n][1][2]);
    QObject::connect(doubleSpinBox_bx, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::connect(doubleSpinBox_by, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
    QObject::connect(doubleSpinBox_bz, SIGNAL(valueChanged(double)), this, SLOT(alterB()));
}

void GuiCtrl::initC(QString part){
    int n = part.toInt();
    QObject::disconnect(doubleSpinBox_cx, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::disconnect(doubleSpinBox_cy, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::disconnect(doubleSpinBox_cz, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    doubleSpinBox_cx->setValue(Db::construct[n][2][0]);
    doubleSpinBox_cy->setValue(Db::construct[n][2][1]);
    doubleSpinBox_cz->setValue(Db::construct[n][2][2]);
    QObject::connect(doubleSpinBox_cx, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::connect(doubleSpinBox_cy, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
    QObject::connect(doubleSpinBox_cz, SIGNAL(valueChanged(double)), this, SLOT(alterC()));
}

void GuiCtrl::initD(QString part){
    int n = part.toInt();
    QObject::disconnect(doubleSpinBox_dx, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::disconnect(doubleSpinBox_dy, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::disconnect(doubleSpinBox_dz, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    doubleSpinBox_dx->setValue(Db::construct[n][3][0]);
    doubleSpinBox_dy->setValue(Db::construct[n][3][1]);
    doubleSpinBox_dz->setValue(Db::construct[n][3][2]);
    QObject::connect(doubleSpinBox_dx, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::connect(doubleSpinBox_dy, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
    QObject::connect(doubleSpinBox_dz, SIGNAL(valueChanged(double)), this, SLOT(alterD()));
}

void GuiCtrl::alterA(){
    Db::construct[modPart][0][0] = doubleSpinBox_ax->value();
    Db::construct[modPart][0][1] = doubleSpinBox_ay->value();
    Db::construct[modPart][0][2] = doubleSpinBox_az->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::alterB(){
    Db::construct[modPart][1][0] = doubleSpinBox_bx->value();
    Db::construct[modPart][1][1] = doubleSpinBox_by->value();
    Db::construct[modPart][1][2] = doubleSpinBox_bz->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::alterC(){
    Db::construct[modPart][2][0] = doubleSpinBox_cx->value();
    Db::construct[modPart][2][1] = doubleSpinBox_cy->value();
    Db::construct[modPart][2][2] = doubleSpinBox_cz->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::alterD(){
    Db::construct[modPart][3][0] = doubleSpinBox_dx->value();
    Db::construct[modPart][3][1] = doubleSpinBox_dy->value();
    Db::construct[modPart][3][2] = doubleSpinBox_dz->value();
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}

void GuiCtrl::newPart(){
    addPartSQL(comboBox_name->currentText());
    selectParts(comboBox_name->currentText());
    addParts();
    textBrowser->append(addPart);
}

void GuiCtrl::savePart(){
    QColor res;
    QString r, g, b;
    QString name = comboBox_name->currentText();
    QString part = comboBox_part->currentText();
    savePartSQL(name, part, convertConstruct(part.toInt()));
    textBrowser->append(savPart);
    if(cdl != NULL){
        res = cdl->selectedColor();
        r = QString::number(res.redF());
        g = QString::number(res.greenF());
        b = QString::number(res.blueF());
        saveColor(name, part, r, g, b);
        textBrowser->append(savColor);
        selectConstruct(comboBox_name->currentText());
    }
}

vector<vector<vector<QString>>> GuiCtrl::convertConstruct(int index){
    vector<vector<vector<QString>>> res;
    for(int i=0; i<1; i++){
        res.push_back(vector<vector<QString>>());
        for(int j=0; j<5; j++){
            res[i].push_back(vector<QString>());
        }
    }
    for(int i=0; i<1; i++){
        for(int j=0; j<(int)Db::construct[i].size(); j++){
            for(int k=0; k<(int)Db::construct[i][j].size(); k++){
                res[i][j].push_back(QString::number(Db::construct[index][j][k]));
            }
        }
    }
    return res;
}

void GuiCtrl::deletePart(){
    deletePartSQL(comboBox_name->currentText(), comboBox_part->currentText());
    resetPartsSQL(comboBox_name->currentText());
    selectParts(comboBox_name->currentText());
    addParts();
    textBrowser->append(delPart);
    textBrowser->append(resPart);
}

void GuiCtrl::newName(){
    QObject::disconnect(comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(parts()));
    QObject::disconnect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
    addName();
    selectNames();
    addNames();
    textBrowser->append(plusName);
    QObject::connect(comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(parts()));
    QObject::connect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
}

void GuiCtrl::delName(){
    QObject::disconnect(comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(parts()));
    QObject::disconnect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
    removeName(comboBox_name->currentText());
    selectNames();
    addNames();
    textBrowser->append(minusName);
    QObject::connect(comboBox_name, SIGNAL(currentIndexChanged(int)), this, SLOT(parts()));
    QObject::connect(comboBox_part, SIGNAL(currentIndexChanged(int)), this, SLOT(choosePart()));
}

void GuiCtrl::colorDialog(){
    if(cdl != NULL){
        delete(cdl);
        cdl = NULL;
    }
    cdl = new QColorDialog();
    cdl->show();
}

void GuiCtrl::setNormal(){
    int l,m,r;
    l = comboBox_nL->currentIndex();
    m = comboBox_nM->currentIndex();
    r = comboBox_nR->currentIndex();
    NORMAL = {l,m,r};
    std::cout<<NORMAL[0]<<" - "<<NORMAL[1]<<" - "<<NORMAL[2]<<" - "<<std::endl;
    vector<float> test = {1,1,1};
    //test.resize(3);
    switch(NORMAL[0]){
     case 0:
        test[0] = doubleSpinBox_ax->value();
        test[1] = doubleSpinBox_ay->value();
        test[2] = doubleSpinBox_az->value();
        break;
     case 1:
        test[0] = doubleSpinBox_bx->value();
        test[1] = doubleSpinBox_by->value();
        test[2] = doubleSpinBox_bz->value();
        break;
     case 2:
        test[0] = doubleSpinBox_cx->value();
        test[1] = doubleSpinBox_cy->value();
        test[2] = doubleSpinBox_cz->value();
        break;
     case 3:
        test[0] = doubleSpinBox_dx->value();
        test[1] = doubleSpinBox_dy->value();
        test[2] = doubleSpinBox_dz->value();
        break;
    }

    vector<float> testo = {1,1,1};
    //testo.resize(3);
    switch(NORMAL[1]){
     case 0:
        testo[0] = doubleSpinBox_ax->value();
        testo[1] = doubleSpinBox_ay->value();
        testo[2] = doubleSpinBox_az->value();
        break;
     case 1:
        testo[0] = doubleSpinBox_bx->value();
        testo[1] = doubleSpinBox_by->value();
        testo[2] = doubleSpinBox_bz->value();
        break;
     case 2:
        testo[0] = doubleSpinBox_cx->value();
        testo[1] = doubleSpinBox_cy->value();
        testo[2] = doubleSpinBox_cz->value();
        break;
     case 3:
        testo[0] = doubleSpinBox_dx->value();
        testo[1] = doubleSpinBox_dy->value();
        testo[2] = doubleSpinBox_dz->value();
        break;
    }

    vector<float> testa = {1,1,1};
    //testa.resize(3);
    switch(NORMAL[2]){
     case 0:
        testa[0] = doubleSpinBox_ax->value();
        testa[1] = doubleSpinBox_ay->value();
        testa[2] = doubleSpinBox_az->value();
        break;
     case 1:
        testa[0] = doubleSpinBox_bx->value();
        testa[1] = doubleSpinBox_by->value();
        testa[2] = doubleSpinBox_bz->value();
        break;
     case 2:
        testa[0] = doubleSpinBox_cx->value();
        testa[1] = doubleSpinBox_cy->value();
        testa[2] = doubleSpinBox_cz->value();
        break;
     case 3:
        testa[0] = doubleSpinBox_dx->value();
        testa[1] = doubleSpinBox_dy->value();
        testa[2] = doubleSpinBox_dz->value();
        break;
    }

    //std::cout<<test[0]<<std::endl;
    vector<float> left = Vector::direction(testo, test);
    vector<float> right = Vector::direction(testo, testa);
    vector<float> normal = Vector::normalize(Vector::normal(left,right));
    cout<<normal[0]<<" - "<<normal[1]<<" - "<<normal[2]<<" - "<<std::endl;

    updateNormal(comboBox_name->currentText(),comboBox_part->currentText(), QString::number(normal[0]),QString::number(normal[1]),QString::number(normal[2]));
    textBrowser->append(addNormal+" ["+QString::number(normal[0])+"|"+QString::number(normal[1])+"|"+QString::number(normal[2])+"]");
    selectNormal(comboBox_name->currentText());
    view.paintGL(1.0, rotation, comboBox_part->currentText());
}
