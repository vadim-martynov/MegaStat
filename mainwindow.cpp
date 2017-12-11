#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef WIN32
    confPath = "c:/VoiceComm/conf";
#else
    confPath = "/data/etc/Mega/VoiceComm/conf";
#endif
    this->setWindowTitle(TITLE_MAIN);
    connect(ui->actionSelect_system, SIGNAL(triggered()), this, SLOT(menu_open_click()));
    connect(ui->actionSave_text_report, SIGNAL(triggered()), this, SLOT(menu_save_click()));
    connect(ui->statusBar, SIGNAL(messageChanged(QString)), this, SLOT(status_change(QString)));

    ui->twTypes->setColumnWidth(0, 80);
    ui->twTypes->setColumnWidth(1, 80);
    ui->twTypes->setColumnWidth(2, 180);
    starter = new QTimer;
    connect(starter, SIGNAL(timeout()), this, SLOT(starter_tick()));
    starter->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::starter_tick()
{
    starter->stop();
    RunSet();
}

void MainWindow::RunSet()
{
    ui->chAll->setEnabled(false);
    ClearScreen();
    QApplication::processEvents();
    InitTypeClasses();
    InitIntClasses();
    if(FindAbonents())
    {
        ShowInterfaces();
        TypeCalc();
    }
    ui->chAll->setEnabled(true);
}


void MainWindow::InitIntClasses()
{
    intClass.clear();
    QStringList classNames;
    classNames.append("Phone");
    classNames.append("Radio");
    classNames.append("Record");
    classNames.append("WS");
    for(quint8 i=0; i<classNames.size(); i++)
    {
        CIntClass tmp;
        tmp.name = classNames.at(i);
        intClass.append(tmp);
    }

    hostClass.clear();
    for(quint8 i=0; i<5; i++)
        hostClass.append(0);
}

void MainWindow::InitTypeClasses()
{
    intTypes.clear();
    CTypeClass tmp;
    tmp.count = 0;
    tmp.intClass = C_PHOHE;
    tmp.type = 101; tmp.name = tr("PRB");           intTypes.insert(tmp.type, tmp);
    tmp.type = 102; tmp.name = tr("DPU2");          intTypes.insert(tmp.type, tmp);
    tmp.type = 103; tmp.name = tr("DPU3");          intTypes.insert(tmp.type, tmp);
    tmp.type = 104; tmp.name = tr("DPU3 AC");       intTypes.insert(tmp.type, tmp);
    tmp.type = 110; tmp.name = tr("Open chn");           intTypes.insert(tmp.type, tmp);
    tmp.type = 111; tmp.name = tr("IVA");           intTypes.insert(tmp.type, tmp);
    tmp.type = 112; tmp.name = tr("FXO");           intTypes.insert(tmp.type, tmp);
    tmp.type = 113; tmp.name = tr("FXS");           intTypes.insert(tmp.type, tmp);
    tmp.type = 120; tmp.name = tr("R2");           intTypes.insert(tmp.type, tmp);
    tmp.type = 121; tmp.name = tr("ISDN PRI");           intTypes.insert(tmp.type, tmp);
    tmp.type = 123; tmp.name = tr("Radio mng");           intTypes.insert(tmp.type, tmp);
    tmp.type = 130; tmp.name = tr("Alarm call");           intTypes.insert(tmp.type, tmp);
    tmp.type = 302; tmp.name = tr("Server trm");           intTypes.insert(tmp.type, tmp);
    tmp.type = 401; tmp.name = tr("Open chn Ladoga");           intTypes.insert(tmp.type, tmp);
    tmp.type = 402; tmp.name = tr("Phone Ladoga");           intTypes.insert(tmp.type, tmp);
    tmp.type = 403; tmp.name = tr("Radio mng Ladoga");           intTypes.insert(tmp.type, tmp);
    tmp.type = 423; tmp.name = tr("Phone ED-137");           intTypes.insert(tmp.type, tmp);
    tmp.type = 451; tmp.name = tr("Open chn E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 452; tmp.name = tr("DPU3 AC E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 453; tmp.name = tr("IVA E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 454; tmp.name = tr("Phone Nabat");           intTypes.insert(tmp.type, tmp);
    tmp.type = 455; tmp.name = tr("FXO Nabat");           intTypes.insert(tmp.type, tmp);
    tmp.type = 456; tmp.name = tr("FXS Nabat");           intTypes.insert(tmp.type, tmp);
    tmp.type = 499; tmp.name = tr("Server trm E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 501; tmp.name = tr("Open chn QSIG");           intTypes.insert(tmp.type, tmp);
    tmp.type = 502; tmp.name = tr("Phone QSIG");           intTypes.insert(tmp.type, tmp);
    tmp.type = 503; tmp.name = tr("Radio mng QSIG");           intTypes.insert(tmp.type, tmp);
    tmp.type = 511; tmp.name = tr("Server trm QSIG");           intTypes.insert(tmp.type, tmp);

    tmp.intClass = C_RADIO;
    tmp.type = 201; tmp.name = tr("Radio DC");           intTypes.insert(tmp.type, tmp);
    tmp.type = 202; tmp.name = tr("Radio AC");           intTypes.insert(tmp.type, tmp);
    tmp.type = 203; tmp.name = tr("Radio 203");           intTypes.insert(tmp.type, tmp);
    tmp.type = 204; tmp.name = tr("Radio T+");           intTypes.insert(tmp.type, tmp);
    tmp.type = 205; tmp.name = tr("Radio SELCAL");           intTypes.insert(tmp.type, tmp);
    tmp.type = 404; tmp.name = tr("Radio Ladoga");           intTypes.insert(tmp.type, tmp);
    tmp.type = 421; tmp.name = tr("Radio ED-137");           intTypes.insert(tmp.type, tmp);
    tmp.type = 491; tmp.name = tr("Radio CAS E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 492; tmp.name = tr("Radio AC E1");           intTypes.insert(tmp.type, tmp);
    tmp.type = 504; tmp.name = tr("Radio QSIG");           intTypes.insert(tmp.type, tmp);

    tmp.intClass = C_RECORD;
    tmp.type = 350; tmp.name = tr("Recorder");           intTypes.insert(tmp.type, tmp);

    tmp.intClass = C_TERMWS;
    tmp.type = 301; tmp.name = tr("WS M3");           intTypes.insert(tmp.type, tmp);
    tmp.type = 303; tmp.name = tr("WS KLP");           intTypes.insert(tmp.type, tmp);
    tmp.type = 304; tmp.name = tr("WS SB");           intTypes.insert(tmp.type, tmp);


}


bool MainWindow::FindAbonents()
{
    QString screensSuf = "screens";
    quint16 classInt = C_NONE;
    QDir conf(confPath);
    QStringList lstDirs = conf.entryList(QDir::Dirs |
                                         QDir::AllDirs |
                                         QDir::NoDotAndDotDot);
    if(!QFile::exists(confPath + "/tcms.conf"))
    {
        //qDebug() << "empty conf dir";
        ui->statusBar->showMessage(confPath + "\t\t" + tr("Empty configuration directory"));
        return false;
    }

    //qDebug() << "dir size " << lstDirs.size();
    foreach(QString entry, lstDirs)
    {
        if(entry != screensSuf)
        {
            quint8 j = 0;
            bool run;
            CCfgConf hardware;
            hardware.parseFile(confPath + "/" + entry + "/hardware.conf");
            //Host class select
            QString pr = entry.left(2);
            if(pr == "d_")
                hostClass[C_HOST_D]++;
            else if(pr == "e_" || pr == "v_")
                hostClass[C_HOST_E]++;
            else if(pr == "i_")
                hostClass[C_HOST_I]++;
            else if(pr == "w_")
                hostClass[C_HOST_W]++;
            else if(pr == "s_")
            ;
            else
            {
                hostClass[C_HOST_M2]++;
            }

            //ID find
            do
            {
                j++;
                QString interface = GetSection( "Interface", j);
                CIdEntry tmp;
                tmp.type = hardware.getValue(interface, "type").toInt();
                tmp.id = hardware.getValue(interface, "internal_address");
                tmp.name = hardware.getValue(interface, "name").mid(1);
                //qDebug() << "tmp" << tmp.id << tmp.type << tmp.name;

                run = (tmp.type > 0);

                switch(tmp.type)
                {
                case 0: //after last interface
                    classInt = C_NONE;
                    break;
                //radio
                case 201: //radio dc
                case 202: //radio ac
                case 203:
                case 204: //radio t+
                case 205: //radio selcal
                case 404: //radio ladoga
                case 421: //radio ed-137
                case 491: //radio cas
                case 492: //radio ac
                case 504: //radio qsig
                    classInt = C_RADIO;
                    break;
                case 350: //recorder
                    classInt = C_RECORD;
                    break;
                case 101: //
                case 102: //dpu2
                case 103: //(dpu3)
                case 104: //dpu3 ac
                case 110: //open chanell
                case 111: //iva
                case 112: //fxo
                case 113: //fxs
                case 120: //r2
                //case 121: //isdn pri
                case 123: //radio man
                case 130: //avar horn

                case 302: //terminal srv

                case 401: //(open) ladoga
                case 402: //phone ladoga
                case 403: //radio man ladoga
                case 423: //phone ed-137
                case 451: //e1 open
                case 452: //e1 ac
                case 453: //e1 iva
                case 454: //nabat phone
                case 455: //nabat fxo
                case 456: //nabat fxs
                case 499: //e1 terminal srv
                case 501: //(open) qsig
                //case 502: //phone qsig
                case 503: //radio man qsig
                case 511: //ats qsig
                    classInt = C_PHOHE;
                    break;
                case 301: //m3 ws
                case 303: //klp ws
                case 304: //sb ws
                    classInt = C_TERMWS;
                    break;

                default:
                {
                    if(tmp.type != 121 && tmp.type != 502)
                    {
                        qDebug() << "!! other int" << tmp.type << tmp.id << tmp.name;
                        classInt = C_NONE;
                    }
                    break;
                }
                }//switch

                if(classInt != C_NONE && tmp.id.toInt() > 0)
                    intClass[classInt].idMap[tmp.id.toInt()] = tmp;

                //for WS
                if(classInt == C_TERMWS)
                    intTypes[tmp.type].count++;

                //isdn special
                if(tmp.type == 121 || tmp.type == 502)//ISDN
                {
                    classInt = C_PHOHE;
                    for(quint8 slot = 1; slot < 31; slot++)
                    {
                        tmp.id = hardware.getValue(interface + GetSection( "slot_", slot), "internal_address");
                        if(tmp.id.toInt() > 0)
                            intClass[classInt].idMap[tmp.id.toInt()] = tmp;
                    }
                }

            } while(run);//interface on host
        }//no screen
    }//conf dirs

    //Get all ID from screens
    QDir screens(confPath + "/" + screensSuf);
    QStringList lstScreens = screens.entryList(QDir::Files |
                                               QDir::NoDotAndDotDot);
    foreach (QString entry, lstScreens)
    {
        CCfgConf conf;
        CIdEntry tmp;
        conf.parseFile(confPath + "/" + screensSuf + "/" + entry);
        tmp.id = conf.getValue("", "internal_address");
        tmp.type = 1;
        tmp.name = conf.getValue("", "name").mid(1);

        classInt = C_TERMWS;
        if(tmp.id.toInt() > 0)
            intClass[classInt].idMap[tmp.id.toInt()] = tmp;

    }
    return true;
}

void MainWindow::menu_open_click()
{
    //qDebug() << "#menu open";
    QDir vcPath(confPath);
    vcPath.cdUp();
    QString dirname = QFileDialog::getExistingDirectory(
                this,
                tr("Select a Directory"),
                vcPath.path(),
                QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
    if( !dirname.isNull() )
    {
        confPath = dirname;
        RunSet();
    }
}

void MainWindow::menu_save_click()
{
    //qDebug() << "#menu save";
    static QDir repPath;
    if(repPath.absolutePath() == QDir::currentPath())
    {
        repPath.setPath(confPath);
        repPath.cdUp();
    }
    QString sufix = ".txt";
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save text report"),
                repPath.path(),
                //QDir::currentPath(),
                tr("Text report (*.txt)"), 0,
                QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly );
    repPath.setPath(fileName);
    if(!fileName.endsWith(sufix))
        fileName += sufix;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setCodec(RU_WIN);

        QDateTime dt(QDateTime::currentDateTime());
        QString en = "\n";
        QString ta = "\t";
        QString header;
        header = QString(TITLE_MAIN) + " Text Report File" + ta;
        header += dt.toString("dd.MM.yyyy hh:mm:ss") + en;
        header += tr("Conf directory:") + ta + confPath + en;
        header += tr("Conf revision:") + ta + GetRevision();
        out << header;

        out << en + en + tr("Hosts statistics:");
        out << en + ui->lbl_M2->text() + ta + ui->lblM2->text();
        out << en + ui->lbl_Dig->text() + ta + ui->lblDig->text();
        out << en + ui->lbl_Anl->text() + ta + ui->lblAnl->text();
        out << en + ui->lbl_WS3->text() + ta + ui->lblWS3->text();
        out << en + ui->lbl_WS2->text() + ta + ui->lblWS2->text();

        out << en + en + tr("Interfaces statistics:");
        out << en + ui->lbl_Phone->text() + ta + ui->lblPhone->text();
        out << en + ui->lbl_Radio->text() + ta + ui->lblRadio->text();
        out << en + ui->lbl_Rec->text() + ta + ui->lblRec->text();
        out << en + ui->lbl_WS->text() + ta + ui->lblWS->text();

        out << en + en + tr("ID utilization:");
        out << en + ui->lbl_PhoneID->text() + ta + ui->lblPhoneId->text();
        out << en + ui->lbl_RadioID->text() + ta + ui->lblRadioId->text();
        out << en + ui->lbl_RecId->text() + ta + ui->lblRecId->text();

        out << en + en + tr("Interfaces by types:") + en;
        for(quint8 j=0; j<ui->twTypes->columnCount(); j++)
        {
            //qDebug() <<
            out << ui->twTypes->horizontalHeaderItem(j)->text() + ta;
        }
        out << en;
        for(quint16 i=0; i<ui->twTypes->rowCount(); i++)
        {
            for(quint8 j=0; j<ui->twTypes->columnCount(); j++)
            {
                out << ui->twTypes->item(i, j)->text() + ta;
            }
            out << en;
        }
        file.close();
    }
}




void MainWindow::ShowInterfaces()
{
//    foreach (CIntClass entry, intClass)
//    {
//        qDebug() << "#" << entry.name;
//        foreach (CIdEntry entryId, entry.idMap)
//        {
//            qDebug() << "#" << entryId.id << entryId.type << entryId.name;
//        }
//    }
//    foreach (CIntClass entry, intClass)
//    {
//        qDebug() << "#size: " << entry.name << entry.idMap.size();
//    }
//    foreach (quint16 entry, hostClass)
//    {
//        qDebug() << "# host class size: " << entry;
//    }

    ui->lblM2->setText(QString::number(hostClass.at(4)));
    ui->lblDig->setText(QString::number(hostClass.at(1)));
    ui->lblAnl->setText(QString::number(hostClass.at(2)));
    ui->lblWS3->setText(QString::number(hostClass.at(3)));
    ui->lblWS2->setText(QString::number(hostClass.at(0)));


    ui->lblPhone->setText(QString::number(intClass.at(0).idMap.size()));
    ui->lblRadio->setText(QString::number(intClass.at(1).idMap.size()));
    ui->lblRec->setText(QString::number(intClass.at(2).idMap.size()));
    ui->lblWS->setText(QString::number(intClass.at(3).idMap.size()));

    ui->lblPhoneId->setText(QString("%1%").arg((intClass.at(0).idMap.size() + intClass.at(3).idMap.size())*100/1024));
    ui->lblRadioId->setText(QString("%1%").arg(intClass.at(1).idMap.size()*100/255));
    ui->lblRecId->setText(QString("%1%").arg(intClass.at(2).idMap.size()*100/1024));

    ui->statusBar->showMessage(confPath + "\t\t" + GetRevision());
}

void MainWindow::ClearScreen()
{
    ui->twTypes->clearContents();
    ui->twTypes->setRowCount(0);

    ui->lblM2->setText("");
    ui->lblDig->setText("");
    ui->lblAnl->setText("");
    ui->lblWS3->setText("");
    ui->lblWS2->setText("");


    ui->lblPhone->setText("");
    ui->lblRadio->setText("");
    ui->lblRec->setText("");
    ui->lblWS->setText("");

    ui->lblPhoneId->setText("");
    ui->lblRadioId->setText("");
    ui->lblRecId->setText("");

    ui->statusBar->showMessage(tr("Counting..."));

}


void MainWindow::TypeCalc()
{
    foreach (CIntClass entry, intClass)
    {
        //qDebug() << "#" << entry.name;
        foreach (CIdEntry entryId, entry.idMap)
        {
            //qDebug() << "#" << entryId.id << entryId.type << entryId.name;
            if(intTypes[entryId.type].intClass != C_TERMWS)
                intTypes[entryId.type].count++;
        }
    }


    foreach (CTypeClass entry, intTypes)
    {
        //qDebug() << "#" << entry.type << entry.name << entry.count;
        if(entry.type != 0)
        {
            if(entry.count != 0 || ui->chAll->isChecked())
            {
                quint8 row = ui->twTypes->rowCount();
                ui->twTypes->setRowCount(row + 1);
                ui->twTypes->setItem(row, 0, new QTableWidgetItem(intClass.at(entry.intClass).name));
                ui->twTypes->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(entry.type)));
                ui->twTypes->setItem(row, 2, new QTableWidgetItem(entry.name));
                ui->twTypes->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(entry.count)));
                ui->twTypes->setRowHeight(row, 25);
            }
        }
    }
}

QString MainWindow::GetRevision()
{
    QString ret = tr("unknown");
    QFile file(confPath + "/timestamp");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString rev(file.readAll());
        if(rev.size() == 10)
        {
            QDateTime dt;
            dt.setMSecsSinceEpoch(0);
            dt = dt.addSecs(rev.toLongLong());
            ret = dt.toString("dd.MM.yyyy hh:mm:ss");
        }
        else
        {
            ret = rev;
        }
        file.close();
    }
    return ret;
}

QString MainWindow::GetSection(const QString &name, quint8 i)
{
    QString s;
    s = QString::number(i);
    if(i<100)
    {
        s = "00" + s;
        s = s.right(2);
    }
    s = "/" + name + s;
    return s;
}

void MainWindow::on_chAll_toggled(bool checked)
{
    RunSet();
}

void MainWindow::status_change(QString text)
{
    //qDebug() << "#status" << text;
}
