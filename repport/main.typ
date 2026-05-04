// --------------------------------------
// Document Setup
// --------------------------------------

#set page(
  margin: 2cm,
)
// table of contents:
// #outline()

//
// #bibliography("refs.bib")

// ### 1. Better fonts
// ```typst
// #set text(font: "New Computer Modern")
#set text(
  font: "Libertinus Serif",
  size: 11pt,
)

#set heading(
  numbering: "1.",
)

// --------------------------------------
// Title
// --------------------------------------

#align(center)[
  #text(size: 20pt, weight: "bold")[MLP + Parallel Mini-Batch SGD (MNIST)]

  #v(0.1em)

  #text(size: 15pt, weight: "bold")[SINGLE THREADED] \
  #v(0.1em)
  Ait-Ameur Mohamed Ilyas \
  Rayane Khelfa \
  Abada Abderahmane \
  Yanis Belharet \

  #v(0.1em)
  #datetime.today().display("[month repr:long] [day], [year]")
]

#v(2em)

// --------------------------------------
// Content Starts Here
// --------------------------------------

= Introduction

== Problem

The MNIST dataset is is a a subset of a larger set available from NIST. It is a somewhat classic example and often considered to be the "Hello, World!" of machine learning. It's a 'classification' problem in the sense that we are expected to train a neural network to be able to recognize handwritten digits.

== Dataset Description

#v(1em)

#figure(
  box(width: 100%)[
    #grid(
      columns: (1fr, 1fr, 1fr, 1fr),
      gutter: 10pt,
      image("images/train_00007_label3.png"),
      image("images/train_00001_label0.png"),
      image("images/train_00008_label1.png"),
      image("images/train_00002_label4.png"),
    ),
  ],
  caption: [MNIST Dataset Samples],
)

These are images with a very low resolution of $28 times 28$ _greyscale_ pixel values encoded on 8 bits (i.e. with the values of each pixel ranging from $0 "to" 255$).

The dataset is split as follows:

```
dataset/MNIST/
├── t10k-images.idx3-ubyte   -> . Test images
├── t10k-labels.idx1-ubyte   -> . Test labels
├── train-images.idx3-ubyte  -> . Training images
└── train-labels.idx1-ubyte  -> . Training labels
```

The 2 types of files:

=== Image files (idx3):
```
[0..3]   magic number  = 0x00000803 (from byte 0 to byte 3 inclusive)
[4..7]   num_images
[8..11]  num_rows  (28)
[12..15] num_cols  (28)
[16..]   pixels: uint8, row-major, one byte per pixel
```

=== Label files (idx1):
```
[0..3]   magic number = 0x00000801
[4..7]   num_labels
[8..]    labels: uint8, value 0-9
```

The magic number in each filetype serves as a sanity check

== Motivation

= Serial Implementation

== Preprocessing
== Algorithm Description & Complexity Analysis

= Parallel Implementation

== Preprocessing
== Design Rational
== Data/Task Decomposition
== Synchronisation

= Experimental Results

== Timing Table
== Speedp Graph
== Accuracy Comparison

= Conclusion and Perspectives

== Findings
== Limitations
== Future Direction

= References
