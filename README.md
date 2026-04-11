## We went Viral?\!  [Check out the post!](https://www.instagram.com/reel/DP9AHwSjcvt/?utm_source=ig_web_copy_link&igsh=NTc4MTIwNjQ2YQ==)

# Your MacBook is Now a Harmonium

Hi, I'm Vedaant (or **rocktopus101** on GitHub). As a Computer Science grad student at USC, my MacBook is usually for compiling code, writing papers, and fueling a mild caffeine addiction. But I figured, why not make it musical?

This app transforms your MacBook into a surprisingly fun and expressive harmonium. You play the notes on the keyboard, and in a stroke of what is either genius or madness, you **use the laptop's lid as the bellows to pump air**.

This project is a fork and a complete musical reimagining of the original **LidAngleSensor** utility by the brilliant Sam Gold.

-----

## How to Play Your Laptop

The concept is simple, just like a real harmonium: you need air and you need to press a key.

  * **🎹 The Keys:** The bottom two rows of your keyboard are the keys (i.e., `Z`, `X`, `C`... for the white keys; and `S`, `D`, `G`...for the black keys). A handy legend in the app shows you exactly which note each key plays in the selected scale.
  * **💨 The Bellows:** This is the fun part. **Open and close your laptop lid to pump air**. The faster you move it, the more "air pressure" you build, and the louder the notes will be. If you stop pumping, the sound will naturally fade out as the air depletes.
  * **🎼 The Scales:** Not a fan of playing in Chromatic? Use the dropdown to switch between Major, Minor, and other scales to easily create melodies.

-----

## The Obligatory FAQ

**So, what is this, exactly?**
It's an app that proves that with enough programming, you can turn any piece of hardware into a musical instrument. It's also a fantastic way for me to learn about macOS audio programming instead of studying for my finals.

**Wait, the LID is the bellows? How?**
Yep. MacBooks have a hidden lid angle sensor that reports its exact position. I'm using the *velocity* of the lid movement to simulate pumping air into a virtual reservoir. It's the most fun you can have with a hinge.

**Will it work on my M1 Mac?**
I made and tested this on my M1 pro, so hopefully it does on yours too\!
However, users seem to have troubles with getting it to run on the base M1 Air and touchbar Pro from 2020. :/

**What about my iMac?**
Does it have a lid you can flap? No? Then you might be out of luck. I suppose you could try picking it up and shaking it gently, but my lawyer (and yours) would strongly advise against it.

**Can I help?**
Please do\! Fork the repo, add a tabla machine, make it sound like a sitar, fix my questionable audio mixing—go wild.
Right now, the code doesn't really work for piano style sharper notes so if you could make that work it would be great\!

**Why is the size so big?**
It's the 40 high quality Harmonium sounds.

----
## Star History

<a href="https://www.star-history.com/?repos=rocktopus101%2Fhingemonium&type=date&logscale=&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/chart?repos=rocktopus101/hingemonium&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/chart?repos=rocktopus101/hingemonium&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/chart?repos=rocktopus101/hingemonium&type=date&legend=top-left" />
 </picture>
</a>
-----

## Origins & Big Thanks

This project stands on the shoulders of a giant. It would not exist without the original **LidAngleSensor** utility created by **Sam Gold**. He did the hard work of discovering the sensor and building the original app. I just put a musical spin on it. All credit for the foundational concept goes to him. You should check out his work\!

## Brew Installation

Simply run the following commands in the terminal!
```
brew tap Rocktopus101/homebrew-personal

brew install hingemonium
```
 just download the latest release\!

## Building It

You'll need Xcode. Clone the repo, hit the big triangle play button, and you should be good to go.
