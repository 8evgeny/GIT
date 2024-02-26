#!/bin/bash
psql -d niokrDB -U postgres
psql -c "CREATE TABLE IF NOT EXISTS WEB_CONTENT (folder TEXT,oboznachenieIkodDokumenta TEXT,naimenovanieIzdeliya  TEXT,naimenovanieDokumenta TEXT,changeNumStr TEXT,notificationDataStr TEXT,controlSummOrigin TEXT,infoOrderList TEXT,created TEXT);"
exit


