SUBDIRS = man

bin_PROGRAMS = shairport-sync
shairport_sync_SOURCES = shairport.cpp rtsp.cpp mdns.cpp mdns_external.cpp common.cpp rtp.cpp player.cpp alac.cpp audio.cpp 

AM_CPPFLAGS = -Wno-multichar -DSYSCONFDIR=\"$(sysconfdir)\"

if USE_CUSTOMPIDDIR
AM_CPPFLAGS+= \
	-DPIDDIR=\"$(CUSTOM_PID_DIR)\"
endif

if USE_AVAHI
shairport_sync_SOURCES += mdns_avahi.cpp
endif

if USE_TINYSVCMDNS
shairport_sync_SOURCES += mdns_tinysvcmdns.cpp tinysvcmdns.cpp
endif

if USE_ALSA
shairport_sync_SOURCES += audio_alsa.cpp
endif

if USE_SNDIO
shairport_sync_SOURCES += audio_sndio.cpp
endif

if USE_STDOUT
shairport_sync_SOURCES += audio_stdout.cpp
endif

if USE_PIPE
shairport_sync_SOURCES += audio_pipe.cpp
endif

if USE_DUMMY
shairport_sync_SOURCES += audio_dummy.cpp
endif

if USE_AO
shairport_sync_SOURCES += audio_ao.cpp
endif

if USE_PULSE
shairport_sync_SOURCES += audio_pulse.cpp
endif

if USE_DNS_SD
shairport_sync_SOURCES += mdns_dns_sd.cpp
endif

install-exec-hook:
if INSTALL_CONFIG_FILES
	[ -e $(DESTDIR)$(sysconfdir) ] || mkdir $(DESTDIR)$(sysconfdir)
	cp scripts/shairport-sync.conf $(DESTDIR)$(sysconfdir)/shairport-sync.conf.sample
	[ -f $(DESTDIR)$(sysconfdir)/shairport-sync.conf ] || cp scripts/shairport-sync.conf $(DESTDIR)$(sysconfdir)/shairport-sync.conf
endif
if INSTALL_SYSTEMV
	[ -e $(DESTDIR)$(sysconfdir)/init.d ] || mkdir -p $(DESTDIR)$(sysconfdir)/init.d
	[ -f $(DESTDIR)$(sysconfdir)/init.d/shairport-sync ] || cp scripts/shairport-sync $(DESTDIR)$(sysconfdir)/init.d/
endif
if INSTALL_SYSTEMD
	[ -e $(DESTDIR)$(systemdsystemunitdir) ] || mkdir -p $(DESTDIR)$(systemdsystemunitdir)
	[ -f $(DESTDIR)$(systemdsystemunitdir)/shairport-sync.service ] || cp scripts/shairport-sync.service $(DESTDIR)$(systemdsystemunitdir)
endif
