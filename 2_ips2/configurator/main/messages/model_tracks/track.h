#pragma once
#include <QString>
#include <QFileInfo>
#include "include/typedefs.h"

class Track {
public:
   Track (QString const& path = {}, bool status = false, bool is_play = false)
       : m_path(path)
       , m_is_checked(status)
       , m_is_play(is_play)
   {
   }
   void setPath(QString const& path) noexcept {
       m_path = path;
   }
   void setIsPlay(bool is) noexcept {
       m_is_play = is;
   }
   void setName(QString const& name) noexcept {       
       m_name = name;
   }
   void setPretone(QString const& pretone) noexcept {
       m_pretone = pretone;
   }
   void setHash(QString const& hash) noexcept {
       m_hash = hash;
   }
   void setChecked(bool is) noexcept {
       m_is_checked = is;
   }
   bool isPlay() const noexcept {
       return m_is_play;
   }
   QString name() const noexcept {
       return QFileInfo(m_path).baseName();
   }
   QString const& path() const noexcept {
       return m_path;
   }
   QString nameTrack() const noexcept {
       return m_name;
   }
   QString const& pretone() noexcept {
       return m_pretone;
   }
   QString const& toHash() noexcept {
       return m_hash;
   }
   bool isChecked() noexcept {
       return m_is_checked;
   }

private:  
  QString m_hash;
  QString m_pretone;
  QString m_path;
  QString m_name;
  bool m_is_checked;
  bool m_is_play;  
};
