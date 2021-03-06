#ifndef KAFFEINESC_H
#define KAFFEINESC_H

#include <qptrlist.h>
#include <qvaluelist.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <qcheckbox.h>
#include <qtimer.h>

#include <kdialogbase.h>
#include <klistview.h>
#include <kaffeine/kaffeinedvbplugin.h>

#include "cardclient.h"
#include "tpsau.h"
#include "emm.h"
#include "scconfigdialogui.h"



class ConfigLine
{
public:
	ConfigLine() {};
	ConfigLine( QString, QString, QString, int, QString, QString, QString );
	QString host, user, pass, key, caid, prov;
	int port;
};



class ScListViewItem : public KListViewItem
{
public:
	ScListViewItem( CardClient *c, KListView *parent, QString host, QString user, QString pass, QString port, QString key, QString caid, QString prov );

	CardClient *cc;
};



class KaffeineSc;

class ScConfigDialog : public ScConfigDialogUI
{
	Q_OBJECT

public:
	ScConfigDialog( KaffeineSc *k, QWidget *parent, QPtrList<CardClient> *cc );
	~ScConfigDialog() {};

	static QValueList<ConfigLine> getNewcsConf();
	void saveNewcsConf( QValueList<ConfigLine> list );
	void loadKeyFile();
	void loadECMinfo();

private:
	QPtrList<CardClient> *csList;
	KaffeineSc *ksc;

protected slots:
	void accept();
	void addEntry();
	void deleteEntry();
	void saveKeyFile();
	void clientChanged( QListViewItem *it);
	void gboxEnabled( bool b );
	void ccamEnabled( bool b );
	void loadECMinfoBtn();

signals:
	void removeCardClient( CardClient* );
	void saveSoftcamKey( const QString &text );
};



 class KaffeineSc : public KaffeineDvbPlugin
{
	Q_OBJECT
public:

	KaffeineSc( QWidget*, const char*, QObject*, const char*, const QStringList& );
	~KaffeineSc();
	QString pluginName();

	void* init( int sid , int anum, int tnum, int fta );

	void process( void* handle, unsigned char* buf, int len );
	void close( void* handle );

	static KAboutData* createAboutData();

public slots:

	void configDialog();
	void killCardClient( CardClient *cc );
	void removeCardClient( CardClient *cc );

private slots:

	void runTpsAu( int anum, int tnum );
	void runTpsAu();
	void newKey( const QStringList & );
	void saveSoftcamKey( const QString &text );

private:

	//void initActions();
	void descramble( unsigned char *cw, unsigned char *buf, int count );

	QMutex mutex, scMutex;
	QPtrList<CardClient> csList;
	QPtrList<CatParser> emmThreads;
	TpsAu tpsau;
	QTimer tpsauTimer;
};
#endif
