/***************************************************************************
 *   Copyright (C) 2008-2012 by Andrzej Rybczak                            *
 *   electricityispower@gmail.com                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include "mutable_song.h"
#include "utility/string.h"

namespace MPD {//

std::string MutableSong::getArtist(unsigned idx) const
{
	return getTag(MPD_TAG_ARTIST, [this, idx](){ return Song::getArtist(idx); }, idx);
}

std::string MutableSong::getTitle(unsigned idx) const
{
	return getTag(MPD_TAG_TITLE, [this, idx](){ return Song::getTitle(idx); }, idx);
}

std::string MutableSong::getAlbum(unsigned idx) const
{
	return getTag(MPD_TAG_ALBUM, [this, idx](){ return Song::getAlbum(idx); }, idx);
}

std::string MutableSong::getAlbumArtist(unsigned idx) const
{
	return getTag(MPD_TAG_ALBUM_ARTIST, [this, idx](){ return Song::getAlbumArtist(idx); }, idx);
}

std::string MutableSong::getTrack(unsigned idx) const
{
	std::string track = getTag(MPD_TAG_TRACK, [this, idx](){ return Song::getTrack(idx); }, idx);
	if ((track.length() == 1 && track[0] != '0')
	||  (track.length() > 3  && track[1] == '/'))
		return "0"+track;
	else
		return track;
}

std::string MutableSong::getDate(unsigned idx) const
{
	return getTag(MPD_TAG_DATE, [this, idx](){ return Song::getDate(idx); }, idx);
}

std::string MutableSong::getGenre(unsigned idx) const
{
	return getTag(MPD_TAG_GENRE, [this, idx](){ return Song::getGenre(idx); }, idx);
}

std::string MutableSong::getComposer(unsigned idx) const
{
	return getTag(MPD_TAG_COMPOSER, [this, idx](){ return Song::getComposer(idx); }, idx);
}

std::string MutableSong::getPerformer(unsigned idx) const
{
	return getTag(MPD_TAG_PERFORMER, [this, idx](){ return Song::getPerformer(idx); }, idx);
}

std::string MutableSong::getDisc(unsigned idx) const
{
	return getTag(MPD_TAG_DISC, [this, idx](){ return Song::getDisc(idx); }, idx);
}

std::string MutableSong::getComment(unsigned idx) const
{
	return getTag(MPD_TAG_COMMENT, [this, idx](){ return Song::getComment(idx); }, idx);
}

void MutableSong::setArtist(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_ARTIST, Song::getArtist(idx), value, idx);
}

void MutableSong::setTitle(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_TITLE, Song::getTitle(idx), value, idx);
}

void MutableSong::setAlbum(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_ALBUM, Song::getAlbum(idx), value, idx);
}

void MutableSong::setAlbumArtist(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_ALBUM_ARTIST, Song::getAlbumArtist(idx), value, idx);
}

void MutableSong::setTrack(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_TRACK, Song::getTrack(idx), value, idx);
}

void MutableSong::setDate(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_DATE, Song::getDate(idx), value, idx);
}

void MutableSong::setGenre(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_GENRE, Song::getGenre(idx), value, idx);
}

void MutableSong::setComposer(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_COMPOSER, Song::getComposer(idx), value, idx);
}

void MutableSong::setPerformer(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_PERFORMER, Song::getPerformer(idx), value, idx);
}

void MutableSong::setDisc(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_DISC, Song::getDisc(idx), value, idx);
}

void MutableSong::setComment(const std::string &value, unsigned idx)
{
	replaceTag(MPD_TAG_COMMENT, Song::getComment(idx), value, idx);
}

const std::string &MutableSong::getNewURI() const
{
	return m_uri;
}

void MutableSong::setNewURI(const std::string &value)
{
	std::string orig_uri = getURI();
	if (orig_uri == value)
		m_uri.clear();
	else
		m_uri = value;
}

void MutableSong::setTag(SetFunction set, const std::string &value, const std::string &delimiter)
{
	auto tags = split(value, delimiter);
	for (size_t i = 0; i < tags.size(); ++i)
		(this->*set)(tags[i], i);
}

bool MutableSong::isModified() const
{
	return !m_uri.empty() && !m_tags.empty();
}

void MutableSong::clearModifications()
{
	m_uri.clear();
	m_tags.clear();
}

std::string MutableSong::getTag(mpd_tag_type tag_type, std::function<std::string()> orig_value, unsigned idx) const
{
	auto it = m_tags.find(Tag(tag_type, idx));
	std::string result;
	if (it == m_tags.end())
		result = orig_value();
	else
		result = it->second;
	return result;
}

void MutableSong::replaceTag(mpd_tag_type tag_type, std::string &&orig_value, const std::string &value, unsigned idx)
{
	Tag tag(tag_type, idx);
	if (value == orig_value)
	{
		auto it = m_tags.find(tag);
		if (it != m_tags.end())
			m_tags.erase(it);
	}
	else
		m_tags[tag] = value;
}

}