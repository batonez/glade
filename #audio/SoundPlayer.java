package ru.hatchery.glade.audio;

import java.util.Collection;

public interface SoundPlayer
{
	void load(Sound sound);
	void hold(Sound sound);
	void hold(Collection<Sound> sounds);
	void unholdAll();
	void clear(boolean force);
	void process();
}
