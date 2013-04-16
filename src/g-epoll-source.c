/*
 ============================================================================
 Name        : g-epoll-source.c
 Author      : Heiher <admin@heiher.info>
 Version     : 0.0.1
 Copyright   : Copyright (C) 2013 everyone.
 Description : 
 ============================================================================
 */

#include <unistd.h>

#include "g-epoll-source.h"

struct _GEpollSource
{
	GSource source;
	
	GPollFD epoll;
	struct epoll_event *events;
	gint nevents;
	gint revents;
	gint timeout;
};

static gboolean g_epoll_source_prepare (GSource *source,
			gint *timeout_);
static gboolean g_epoll_source_check (GSource *source);
static gboolean g_epoll_source_dispatch (GSource *source,
			GSourceFunc callback, gpointer user_data);
static void g_epoll_source_finalize (GSource *source);

static GSourceFuncs g_epoll_source_funcs =
{
	g_epoll_source_prepare,
	g_epoll_source_check,
	g_epoll_source_dispatch,
	g_epoll_source_finalize
};

GSource *
g_epoll_source_new (gint size, gint timeout)
{
	GSource *source;
	GEpollSource *self;

	source = g_source_new (&g_epoll_source_funcs,
				sizeof (GEpollSource));
	self = (GEpollSource *) source;

	self->epoll.fd = epoll_create (size);
	self->epoll.events = G_IO_IN;
	g_source_add_poll (source, &self->epoll);

	self->events = g_malloc0_n (size, sizeof (struct epoll_event));
	self->nevents = size;
	self->timeout = timeout;

	return source;
}

gint
g_epoll_source_add_fd (GSource *source,
			gint fd, GEpollEvent *event)
{
	GEpollSource *self = (GEpollSource *) source;

	return epoll_ctl (self->epoll.fd, EPOLL_CTL_ADD,
				fd, event);
}

gint
g_epoll_source_mod_fd (GSource *source,
			gint fd, GEpollEvent *event)
{
	GEpollSource *self = (GEpollSource *) source;

	return epoll_ctl (self->epoll.fd, EPOLL_CTL_MOD,
				fd, event);
}

gint
g_epoll_source_del_fd (GSource *source,
			gint fd)
{
	GEpollSource *self = (GEpollSource *) source;

	return epoll_ctl (self->epoll.fd, EPOLL_CTL_DEL,
				fd, NULL);
}

static gboolean
g_epoll_source_prepare (GSource *source,
			gint *timeout_)
{
	GEpollSource *self = (GEpollSource *) source;

	self->revents = epoll_wait (self->epoll.fd,
				self->events, self->nevents, 0);
	*timeout_ = self->timeout;

	return (self->revents > 0);
}

static gboolean
g_epoll_source_check (GSource *source)
{
	GEpollSource *self = (GEpollSource *) source;

	self->revents = epoll_wait (self->epoll.fd,
				self->events, self->nevents, 0);

	return (self->revents > 0);
}

static gboolean
g_epoll_source_dispatch (GSource *source,
			GSourceFunc callback, gpointer user_data)
{
	GEpollSource *self = (GEpollSource *) source;
	GEpollSourceFunc es_callback = (GEpollSourceFunc) callback;

	if (!callback)
	  return FALSE;

	return es_callback (self, self->events, self->revents, user_data);
}

static void
g_epoll_source_finalize (GSource *source)
{
	GEpollSource *self = (GEpollSource *) source;

	g_free (self->events);
	close (self->epoll.fd);
}

