#include "mpd.h"

#include <mpd/client.h>
#include <stdio.h>

#include "util.h"

static char *icons[] = {"ﱙ  Unknown", "  Stopped", " ", " "};
static struct mpd_connection *conn = NULL;

int mpd_status(char *output, unsigned int size, const Arg *arg) {
	struct mpd_status *status;
	struct mpd_song *song;
	enum mpd_state state;

	int written = 0, song_id;
	const char *artist, *title, *uri;

	if (conn == NULL) {
		conn = mpd_connection_new(NULL, 0, 0);
	}

	status = mpd_run_status(conn);
	state = mpd_status_get_state(status);

	if (!status)
		return snprintf(output, size, "ﱙ  N/A");
	if (state == MPD_STATE_UNKNOWN || state == MPD_STATE_STOP) {
		written = xstrncpy(output, icons[state], size);
	} else {
		song_id = mpd_status_get_song_id(status);
		song = mpd_run_get_queue_song_id(conn, song_id);
		artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
		title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
		uri = mpd_song_get_uri(song);

		if (artist && title)
			written =
				snprintf(output, size, "%s %s - %s", icons[state], artist, title);
		else
			written =
				snprintf(output, size, "%s %s", icons[state], title ? title : uri);

		mpd_song_free(song);
	}

	mpd_status_free(status);

	return written;
}

void mpd_status_clean(void *arg) {
	if (conn != NULL)
		mpd_connection_free(conn);
}
