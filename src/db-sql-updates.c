/*
 * $Id: $
 * Commands to update database to new version
 *
 */

#include <stdio.h>

char *db_sqlite_updates[] = {
    /* version 0 -> version 1 -- initial update */
    "CREATE TABLE songs (\n"
    "   id              INTEGER PRIMARY KEY NOT NULL,\n"
    "   path            VARCHAR(4096) UNIQUE NOT NULL,\n"
    "   fname           VARCHAR(255) NOT NULL,\n"
    "   title           VARCHAR(1024) DEFAULT NULL,\n"
    "   artist          VARCHAR(1024) DEFAULT NULL,\n"
    "   album           VARCHAR(1024) DEFAULT NULL,\n"
    "   genre           VARCHAR(255) DEFAULT NULL,\n"
    "   comment         VARCHAR(4096) DEFAULT NULL,\n"
    "   type            VARCHAR(255) DEFAULT NULL,\n"
    "   composer        VARCHAR(1024) DEFAULT NULL,\n"
    "   orchestra       VARCHAR(1024) DEFAULT NULL,\n"
    "   conductor       VARCHAR(1024) DEFAULT NULL,\n"
    "   grouping        VARCHAR(1024) DEFAULT NULL,\n"
    "   url             VARCHAR(1024) DEFAULT NULL,\n"
    "   bitrate         INTEGER DEFAULT 0,\n"
    "   samplerate      INTEGER DEFAULT 0,\n"
    "   song_length     INTEGER DEFAULT 0,\n"
    "   file_size       INTEGER DEFAULT 0,\n"
    "   year            INTEGER DEFAULT 0,\n"
    "   track           INTEGER DEFAULT 0,\n"
    "   total_tracks    INTEGER DEFAULT 0,\n"
    "   disc            INTEGER DEFAULT 0,\n"
    "   total_discs     INTEGER DEFAULT 0,\n"
    "   bpm             INTEGER DEFAULT 0,\n"
    "   compilation     INTEGER DEFAULT 0,\n"
    "   rating          INTEGER DEFAULT 0,\n"
    "   play_count      INTEGER DEFAULT 0,\n"
    "   data_kind       INTEGER DEFAULT 0,\n"
    "   item_kind       INTEGER DEFAULT 0,\n"
    "   description     INTEGER DEFAULT 0,\n"
    "   time_added      INTEGER DEFAULT 0,\n"
    "   time_modified   INTEGER DEFAULT 0,\n"
    "   time_played     INTEGER DEFAULT 0,\n"
    "   db_timestamp    INTEGER DEFAULT 0,\n"
    "   disabled        INTEGER DEFAULT 0,\n"
    "   sample_count    INTEGER DEFAULT 0,\n"
    "   force_update    INTEGER DEFAULT 0\n"
    ");\n"
    "CREATE INDEX idx_path ON songs(path);\n" 
    "CREATE TABLE config (\n"
    "   term            VARCHAR(255)    NOT NULL,\n"
    "   subterm         VARCHAR(255)    DEFAULT NULL,\n"
    "   value           VARCHAR(1024)   NOT NULL\n"
    ");\n"
    "CREATE TABLE playlists (\n"
    "   id             INTEGER PRIMARY KEY NOT NULL,\n"
    "   title          VARCHAR(255) NOT NULL,\n"
    "   smart          INTEGER NOT NULL,\n"
    "   items          INTEGER NOT NULL,\n"
    "   query          VARCHAR(1024)\n"
    ");\n"
    "CREATE TABLE playlistitems (\n"
    "   id              INTEGER NOT NULL,\n"
    "   songid         INTEGER NOT NULL\n"
    ");\n"
    "INSERT INTO config VALUES ('version','','1');\n"
    "INSERT INTO playlists VALUES (1,'Library',1,0,'1');\n",

    /* version 1 -> version 2 */
    /* force rescan for invalid utf-8 data */
    "REPLACE INTO config VALUES('rescan',NULL,1);\n"
    "UPDATE config SET value=2 WHERE term='version';\n",

    /* version 2 -> version 3 */
    /* add daap.songcodectype, normalize daap.songformat and daap.songdescription */
    "drop index idx_path;\n"
    "create temp table tempsongs as select * from songs;\n"
    "drop table songs;\n"
    "CREATE TABLE songs (\n"
    "   id              INTEGER PRIMARY KEY NOT NULL,\n"
    "   path            VARCHAR(4096) UNIQUE NOT NULL,\n"
    "   fname           VARCHAR(255) NOT NULL,\n"
    "   title           VARCHAR(1024) DEFAULT NULL,\n"
    "   artist          VARCHAR(1024) DEFAULT NULL,\n"
    "   album           VARCHAR(1024) DEFAULT NULL,\n"
    "   genre           VARCHAR(255) DEFAULT NULL,\n"
    "   comment         VARCHAR(4096) DEFAULT NULL,\n"
    "   type            VARCHAR(255) DEFAULT NULL,\n"
    "   composer        VARCHAR(1024) DEFAULT NULL,\n"
    "   orchestra       VARCHAR(1024) DEFAULT NULL,\n"
    "   conductor       VARCHAR(1024) DEFAULT NULL,\n"
    "   grouping        VARCHAR(1024) DEFAULT NULL,\n"
    "   url             VARCHAR(1024) DEFAULT NULL,\n"
    "   bitrate         INTEGER DEFAULT 0,\n"
    "   samplerate      INTEGER DEFAULT 0,\n"
    "   song_length     INTEGER DEFAULT 0,\n"
    "   file_size       INTEGER DEFAULT 0,\n"
    "   year            INTEGER DEFAULT 0,\n"
    "   track           INTEGER DEFAULT 0,\n"
    "   total_tracks    INTEGER DEFAULT 0,\n"
    "   disc            INTEGER DEFAULT 0,\n"
    "   total_discs     INTEGER DEFAULT 0,\n"
    "   bpm             INTEGER DEFAULT 0,\n"
    "   compilation     INTEGER DEFAULT 0,\n"
    "   rating          INTEGER DEFAULT 0,\n"
    "   play_count      INTEGER DEFAULT 0,\n"
    "   data_kind       INTEGER DEFAULT 0,\n"
    "   item_kind       INTEGER DEFAULT 0,\n"
    "   description     INTEGER DEFAULT 0,\n"
    "   time_added      INTEGER DEFAULT 0,\n"
    "   time_modified   INTEGER DEFAULT 0,\n"
    "   time_played     INTEGER DEFAULT 0,\n"
    "   db_timestamp    INTEGER DEFAULT 0,\n"
    "   disabled        INTEGER DEFAULT 0,\n"
    "   sample_count    INTEGER DEFAULT 0,\n"
    "   force_update    INTEGER DEFAULT 0,\n"
    "   codectype       VARCHAR(5) DEFAULT NULL\n"
    ");\n"
    "begin transaction;\n"
    "insert into songs select *,NULL from tempsongs;\n"
    "commit transaction;\n"
    "update songs set type=lower(type);\n"
    "update songs set type='m4a' where type='aac' or type='mp4';\n"
    "update songs set type='flac' where type='fla';\n"
    "update songs set type='mpc' where type='mpp';\n"
    "update songs set type='mpc' where type='mp+';\n"
    "update songs set description='AAC audio file' where type='m4a';\n"
    "update songs set description='MPEG audio file' where type='mp3';\n"
    "update songs set description='WAV audio file' where type='wav';\n"
    "update songs set description='Playlist URL' where type='pls';\n"
    "update songs set description='Ogg Vorbis audio file' where type='ogg';\n"
    "update songs set description='FLAC audio file' where type='flac';\n"
    "update songs set description='Musepack audio file' where type='mpc';\n"
    "update songs set codectype='mp4a' where type='m4a' or type='m4p';\n"
    "update songs set codectype='mpeg' where type='mp3';\n"
    "update songs set codectype='ogg' where type='ogg';\n"
    "update songs set codectype='flac' where type='flac';\n"
    "update songs set codectype='mpc' where type='mpc';\n"
    "update songs set force_update=1 where type='m4a';\n"      /* look for alac */
    "create index idx_path on songs(path);\n"
    "drop table tempsongs;\n"
    "update config set value=3 where term='version';\n",
    
    /* version 3 -> version 4 */
    /* add db_timestamp and path to playlist table */
    "create temp table tempplaylists as select * from playlists;\n"
    "drop table playlists;\n"
    "CREATE TABLE playlists (\n"
    "   id             INTEGER PRIMARY KEY NOT NULL,\n"
    "   title          VARCHAR(255) NOT NULL,\n"
    "   type           INTEGER NOT NULL,\n"
    "   items          INTEGER NOT NULL,\n"
    "   query          VARCHAR(1024),\n"
    "   db_timestamp   INTEGER NOT NULL,\n"
    "   path           VARCHAR(4096)\n"
    ");\n"
    "insert into playlists select *,0,NULL from tempplaylists;\n"
    "drop table tempplaylists;\n"
    "update config set value=4 where term='version';\n",

    /* version 4 -> version 5 */
    /* add index to playlist table */
    "create temp table tempplaylists as select * from playlists;\n"
    "drop table playlists;\n"
    "CREATE TABLE playlists (\n"
    "   id             INTEGER PRIMARY KEY NOT NULL,\n"
    "   title          VARCHAR(255) NOT NULL,\n"
    "   type           INTEGER NOT NULL,\n"
    "   items          INTEGER NOT NULL,\n"
    "   query          VARCHAR(1024),\n"
    "   db_timestamp   INTEGER NOT NULL,\n"
    "   path           VARCHAR(4096),\n"
    "   idx            INTEGER NOT NULL\n"
    ");\n"
    "insert into playlists select *,0 from tempplaylists;\n"
    "drop table tempplaylists;\n"
    "update config set value=5 where term='version';\n",

    /* version 5 -> version 6 */
    "drop index idx_path;\n"
    "create temp table tempsongs as select * from songs;\n"
    "drop table songs;\n"
    "CREATE TABLE songs (\n"
    "   id              INTEGER PRIMARY KEY NOT NULL,\n"
    "   path            VARCHAR(4096) UNIQUE NOT NULL,\n"
    "   fname           VARCHAR(255) NOT NULL,\n"
    "   title           VARCHAR(1024) DEFAULT NULL,\n"
    "   artist          VARCHAR(1024) DEFAULT NULL,\n"
    "   album           VARCHAR(1024) DEFAULT NULL,\n"
    "   genre           VARCHAR(255) DEFAULT NULL,\n"
    "   comment         VARCHAR(4096) DEFAULT NULL,\n"
    "   type            VARCHAR(255) DEFAULT NULL,\n"
    "   composer        VARCHAR(1024) DEFAULT NULL,\n"
    "   orchestra       VARCHAR(1024) DEFAULT NULL,\n"
    "   conductor       VARCHAR(1024) DEFAULT NULL,\n"
    "   grouping        VARCHAR(1024) DEFAULT NULL,\n"
    "   url             VARCHAR(1024) DEFAULT NULL,\n"
    "   bitrate         INTEGER DEFAULT 0,\n"
    "   samplerate      INTEGER DEFAULT 0,\n"
    "   song_length     INTEGER DEFAULT 0,\n"
    "   file_size       INTEGER DEFAULT 0,\n"
    "   year            INTEGER DEFAULT 0,\n"
    "   track           INTEGER DEFAULT 0,\n"
    "   total_tracks    INTEGER DEFAULT 0,\n"
    "   disc            INTEGER DEFAULT 0,\n"
    "   total_discs     INTEGER DEFAULT 0,\n"
    "   bpm             INTEGER DEFAULT 0,\n"
    "   compilation     INTEGER DEFAULT 0,\n"
    "   rating          INTEGER DEFAULT 0,\n"
    "   play_count      INTEGER DEFAULT 0,\n"
    "   data_kind       INTEGER DEFAULT 0,\n"
    "   item_kind       INTEGER DEFAULT 0,\n"
    "   description     INTEGER DEFAULT 0,\n"
    "   time_added      INTEGER DEFAULT 0,\n"
    "   time_modified   INTEGER DEFAULT 0,\n"
    "   time_played     INTEGER DEFAULT 0,\n"
    "   db_timestamp    INTEGER DEFAULT 0,\n"
    "   disabled        INTEGER DEFAULT 0,\n"
    "   sample_count    INTEGER DEFAULT 0,\n"
    "   force_update    INTEGER DEFAULT 0,\n"
    "   codectype       VARCHAR(5) DEFAULT NULL,\n"
    "   idx             INTEGER NOT NULL\n"
    ");\n"
    "begin transaction;\n"
    "insert into songs select *,0 from tempsongs;\n"
    "commit transaction;\n"
    "create index idx_path on songs(path);\n"
    "drop table tempsongs;\n"
    "update config set value=6 where term='version';\n",

    /* version 6 -> version 7 */
    "create temp table tempitems as select * from playlistitems;\n"
    "drop table playlistitems;\n"
    "CREATE TABLE playlistitems (\n"
    "   id             INTEGER PRIMARY KEY NOT NULL,\n"
    "   playlistid     INTEGER NOT NULL,\n"
    "   songid         INTEGER NOT NULL\n"
    ");\n"
    "insert into playlistitems (playlistid, songid) select * from tempitems;\n"
    "drop table tempitems;\n"
    "update config set value=7 where term='version';\n",
    
    /* version 7 -> version 8 */
    "create index idx_songid on playlistitems(songid)\n"
    "create index idx_playlistid on playlistitems(playlistid)\n"
    "update config set value=8 where term='version';\n",

    /* version 8 -> version 9 */ 
    "drop index idx_path;\n"
    "create temp table tempsongs as select * from songs;\n"
    "drop table songs;\n"
    "CREATE TABLE songs (\n"
    "   id              INTEGER PRIMARY KEY NOT NULL,\n"
    "   path            VARCHAR(4096) UNIQUE NOT NULL,\n"
    "   fname           VARCHAR(255) NOT NULL,\n"
    "   title           VARCHAR(1024) DEFAULT NULL,\n"
    "   artist          VARCHAR(1024) DEFAULT NULL,\n"
    "   album           VARCHAR(1024) DEFAULT NULL,\n"
    "   genre           VARCHAR(255) DEFAULT NULL,\n"
    "   comment         VARCHAR(4096) DEFAULT NULL,\n"
    "   type            VARCHAR(255) DEFAULT NULL,\n"
    "   composer        VARCHAR(1024) DEFAULT NULL,\n"
    "   orchestra       VARCHAR(1024) DEFAULT NULL,\n"
    "   conductor       VARCHAR(1024) DEFAULT NULL,\n"
    "   grouping        VARCHAR(1024) DEFAULT NULL,\n"
    "   url             VARCHAR(1024) DEFAULT NULL,\n"
    "   bitrate         INTEGER DEFAULT 0,\n"
    "   samplerate      INTEGER DEFAULT 0,\n"
    "   song_length     INTEGER DEFAULT 0,\n"
    "   file_size       INTEGER DEFAULT 0,\n"
    "   year            INTEGER DEFAULT 0,\n"
    "   track           INTEGER DEFAULT 0,\n"
    "   total_tracks    INTEGER DEFAULT 0,\n"
    "   disc            INTEGER DEFAULT 0,\n"
    "   total_discs     INTEGER DEFAULT 0,\n"
    "   bpm             INTEGER DEFAULT 0,\n"
    "   compilation     INTEGER DEFAULT 0,\n"
    "   rating          INTEGER DEFAULT 0,\n"
    "   play_count      INTEGER DEFAULT 0,\n"
    "   data_kind       INTEGER DEFAULT 0,\n"
    "   item_kind       INTEGER DEFAULT 0,\n"
    "   description     INTEGER DEFAULT 0,\n"
    "   time_added      INTEGER DEFAULT 0,\n"
    "   time_modified   INTEGER DEFAULT 0,\n"
    "   time_played     INTEGER DEFAULT 0,\n"
    "   db_timestamp    INTEGER DEFAULT 0,\n"
    "   disabled        INTEGER DEFAULT 0,\n"
    "   sample_count    INTEGER DEFAULT 0,\n"
    "   force_update    INTEGER DEFAULT 0,\n"
    "   codectype       VARCHAR(5) DEFAULT NULL,\n"
    "   idx             INTEGER NOT NULL,\n"
    "   has_video       INTEGER DEFAULT 0,\n"
    "   contentrating   INTEGER DEFAULT 0\n"
    ");\n"
    "begin transaction;\n"
    "insert into songs select *,0,0 from tempsongs;\n"
    "commit transaction;\n"
    "update songs set has_video=1 where fname like '%.m4v';\n"
    "create index idx_path on songs(path);\n"
    "drop table tempsongs;\n"
    "update config set value=9 where term='version';\n",

    /* version 9 -> version 10 */ 
    "drop index idx_path;\n"
    "create temp table tempsongs as select * from songs;\n"
    "drop table songs;\n"
    "CREATE TABLE songs (\n"
    "   id              INTEGER PRIMARY KEY NOT NULL,\n"
    "   path            VARCHAR(4096) NOT NULL,\n"
    "   fname           VARCHAR(255) NOT NULL,\n"
    "   title           VARCHAR(1024) DEFAULT NULL,\n"
    "   artist          VARCHAR(1024) DEFAULT NULL,\n"
    "   album           VARCHAR(1024) DEFAULT NULL,\n"
    "   genre           VARCHAR(255) DEFAULT NULL,\n"
    "   comment         VARCHAR(4096) DEFAULT NULL,\n"
    "   type            VARCHAR(255) DEFAULT NULL,\n"
    "   composer        VARCHAR(1024) DEFAULT NULL,\n"
    "   orchestra       VARCHAR(1024) DEFAULT NULL,\n"
    "   conductor       VARCHAR(1024) DEFAULT NULL,\n"
    "   grouping        VARCHAR(1024) DEFAULT NULL,\n"
    "   url             VARCHAR(1024) DEFAULT NULL,\n"
    "   bitrate         INTEGER DEFAULT 0,\n"
    "   samplerate      INTEGER DEFAULT 0,\n"
    "   song_length     INTEGER DEFAULT 0,\n"
    "   file_size       INTEGER DEFAULT 0,\n"
    "   year            INTEGER DEFAULT 0,\n"
    "   track           INTEGER DEFAULT 0,\n"
    "   total_tracks    INTEGER DEFAULT 0,\n"
    "   disc            INTEGER DEFAULT 0,\n"
    "   total_discs     INTEGER DEFAULT 0,\n"
    "   bpm             INTEGER DEFAULT 0,\n"
    "   compilation     INTEGER DEFAULT 0,\n"
    "   rating          INTEGER DEFAULT 0,\n"
    "   play_count      INTEGER DEFAULT 0,\n"
    "   data_kind       INTEGER DEFAULT 0,\n"
    "   item_kind       INTEGER DEFAULT 0,\n"
    "   description     INTEGER DEFAULT 0,\n"
    "   time_added      INTEGER DEFAULT 0,\n"
    "   time_modified   INTEGER DEFAULT 0,\n"
    "   time_played     INTEGER DEFAULT 0,\n"
    "   db_timestamp    INTEGER DEFAULT 0,\n"
    "   disabled        INTEGER DEFAULT 0,\n"
    "   sample_count    INTEGER DEFAULT 0,\n"
    "   force_update    INTEGER DEFAULT 0,\n"
    "   codectype       VARCHAR(5) DEFAULT NULL,\n"
    "   idx             INTEGER NOT NULL,\n"
    "   has_video       INTEGER DEFAULT 0,\n"
    "   contentrating   INTEGER DEFAULT 0\n"
    ");\n"
    "begin transaction;\n"
    "insert into songs select * from tempsongs;\n"
    "commit transaction;\n"
    "create index idx_path on songs(path,idx);\n"
    "drop table tempsongs;\n"
    "update config set value=10 where term='version';\n",
    /* version 10 -> version 11 */ 
    "drop index idx_playlistid;\n"
    "create index idx_playlistid on playlistitems(playlistid,songid);\n"
    "update config set value=11 where term='version';\n",
    /* version 11 -> version 12 */
    "REPLACE INTO config VALUES('rescan',NULL,1);\n"
    "UPDATE config SET value=12 WHERE term='version';\n",
    NULL /* No more versions! */
};
