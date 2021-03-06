#include "test.h"
#include <QtWebSockets>
#include <QtCore>
#include <cstdio>
using namespace std;

QT_USE_NAMESPACE

static QString getIdentifier(QWebSocket *peer)
{
	return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
		QString::number(peer->peerPort()));
}

//Constructeur pour la connexion au serveur
ChatServer::ChatServer(quint16 port, QObject *parent) :
	QObject(parent),
	m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Serveur"),
		QWebSocketServer::NonSecureMode,
		this))
{
	if (m_pWebSocketServer->listen(QHostAddress::AnyIPv4, 500))
	{
		QTextStream(stdout) << "Le serveur ?coute sur le port " << port << '\n';
		connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
			this, &ChatServer::onNewConnection);
	}
}

ChatServer::~ChatServer()
{
	m_pWebSocketServer->close();
}

// Connexion au serveur
void ChatServer::onNewConnection()
{
	auto pSocket = m_pWebSocketServer->nextPendingConnection();
	QTextStream(stdout) << getIdentifier(pSocket) << "Connect?\n";
	pSocket->setParent(this);

	connect(pSocket, &QWebSocket::textMessageReceived,
		this, &ChatServer::processMessage);
	connect(pSocket, &QWebSocket::disconnected,
		this, &ChatServer::socketDisconnected);

	m_clients << pSocket;
}


//Fonction de r?cup?ration des messages
void ChatServer::processMessage(const QString &message)
{
	QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
	for (QWebSocket *pClient : qAsConst(m_clients)) {
		if (pClient != pSender) 
			pClient->sendTextMessage(message);
	}
}


//D?connexion
void ChatServer::socketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	QTextStream(stdout) << getIdentifier(pClient) << "D?connect?\n";
	if (pClient)
	{
		m_clients.removeAll(pClient);
		pClient->deleteLater();
	}
}
