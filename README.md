# ASCII Tower Generator

This is a simple experiment in creating IRC ASCII artwork in a dynamic, procedural fashion.

Rather than actually 'drawing' out an ASCII image, we can write a computer program that generates the character sequences necessary to represent the ASCII itself. 

In this case, I have created a very basic tower that consists of a grayscale gradient, along with a background that is also a gradient. The tower sits on top of a background consisting of either gradient sky -or- that sky overlaid with text from a file given on the command line. 

The program still leaves room for work (we can set up sections to control the color of the sky, control the gradient of color change, clouds/stars, etc) but it gets the general idea across.

## Building

Change into the project directory and run.

```
make
```

## Using

Once the program is built, run:

```
./towergen
```

You should get the output:

```
Usage: ./towergen <rows> [filepath]
```

The way to read this is that you should tell the program how many rows you want to generate, followed by an optional input text file to overlay the tower ontop of. 

------

[https://www.evildojo.com](https://www.evildojo.com)

