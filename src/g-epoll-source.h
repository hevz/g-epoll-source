/*
 ============================================================================
 Name        : g-epoll-source.h
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (C) 2013 everyone.
 Description : 
 ============================================================================
 */

#ifndef __G_EPOLL_SOURCE_H__
#define __G_EPOLL_SOURCE_H__

#include <sys/epoll.h>

#include <glib.h>

G_BEGIN_DECLS

typedef struct epoll_event GEpollEvent;
typedef struct _GEpollSource GEpollSource;

typedef gboolean (*GEpollSourceFunc) (GEpollSource *source,
			GEpollEvent *events, gint nevents,
			gpointer user_data);

GSource * g_epoll_source_new (gint size, gint timeout);

gint g_epoll_source_add_fd (GSource *source,
			gint fd, GEpollEvent *event);
gint g_epoll_source_mod_fd (GSource *source,
			gint fd, GEpollEvent *event);
gint g_epoll_source_del_fd (GSource *source,
			gint fd);

G_END_DECLS

#endif /* __G_EPOLL_SOURCE_H__ */

