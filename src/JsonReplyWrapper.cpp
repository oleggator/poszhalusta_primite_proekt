#include <QtCore/QJsonDocument>
#include "JsonReplyWrapper.h"

JsonReplyWrapper::JsonReplyWrapper(QObject *parent)
  : QObject(parent) {
}

void JsonReplyWrapper::watchReplyState(){
 QJsonDocument doc = QJsonDocument::fromJson( _reply->readAll());
 _jsonResponse = doc.object();
 emit finished();
}

QNetworkReply *JsonReplyWrapper::getReply() const {
  return _reply;
}

void JsonReplyWrapper::setReply(QNetworkReply *reply) {
  _reply = reply;
  QObject::connect(_reply,&QNetworkReply::finished,this,&JsonReplyWrapper::watchReplyState);
  _reply->setParent(this);
}

JsonReplyWrapper::~JsonReplyWrapper() {
  _reply->deleteLater();
}

const QJsonObject &JsonReplyWrapper::getResponse() const {
  return _jsonResponse;
}

JsonReplyWrapper::JsonReplyWrapper(QNetworkReply *reply, QObject *parent)
  :_reply{reply}{
   QObject::connect(_reply,&QNetworkReply::finished,this,&JsonReplyWrapper::watchReplyState);
  _reply->setParent(this);
}

bool JsonReplyWrapper::isError() const {
  return _jsonResponse["error"].isNull() || _reply->error();
}
