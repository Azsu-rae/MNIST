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

#let cell(lang, body) = block(
  width: 100%,
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
  #block(
    width: 100%,
    fill: luma(245),
    inset: (x: 10pt, y: 6pt),
  )[
    #text(size: 9pt, fill: luma(120), font: "monospace")[#lang]
  ]
  #block(
    width: 100%,
    inset: (x: 12pt, y: 10pt),
  )[
    #raw(block: true, lang: lang, body)
  ]
]

// --------------------------------------
// Title
// --------------------------------------

#align(center)[
  #text(size: 20pt, weight: "bold")[MLP + Parallel Mini-Batch SGD (MNIST)]

  #v(0.1em)

  #text(size: 15pt, weight: "bold")[TEAM: SINGLE THREADED] \
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

Let's look at the training files:

=== Image file (idx3):
```
[0..3]   magic number  = 0x00000803 (from byte 0 to byte 3 inclusive)
[4..7]   num_images (60_000)
[8..11]  num_rows  (28)
[12..15] num_cols  (28)
[16..]   pixels: uint8, row-major, one byte per pixel
```

=== Label file (idx1):
```
[0..3]   magic number = 0x00000801
[4..7]   num_labels (60_000)
[8..]    labels: uint8, value 0-9
```

The magic number in each filetype serves as a sanity check since the dataset is written in BigEndian byte notation, it helps to make sure we are reading correctly. They also express the data type (0x08, unsigned byte) and the dimensionality (0x03, 60_000 2D images making this tensor 3D).

== Motivation

Educating ourselves on neural networks and gradient descent learning algorithms as well as their parallelization.

= Serial Implementation

== Preprocessing

=== Data pipeline

I structured and read the files in a single contiguous memory block:

#block(
  width: 100%,
  inset: (x: 12pt, y: 10pt),
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
```c
typedef struct Dataset {
    size_t size;
    Shape shape;
    uint8_t* labels;
    uint8_t* content;
    Image (*index)(Dataset* dataset, size_t index);
} Dataset;
```
]

the `index()` function gets a specific image out of the `content` field which is is the entire dataset placed in a single contiguous memory block. The `Image` type is

#block(
  width: 100%,
  inset: (x: 12pt, y: 10pt),
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
```c
typedef struct Image {
    Shape shape;
    uint8_t* lbl;
    uint8_t* content;
    void (*label)(Image img, uint8_t label);
    uint8_t (*get)(Image img, size_t row, size_t col);
    void (*set)(Image img, size_t row, size_t col, uint8_t value);

    void (*print)(Image img);
} Image;
```
]

which as it turns out is mostly just development utilites rather than userful algorithmic tools. The `Shape` is mostly as you can expect:

#block(
  width: 100%,
  inset: (x: 12pt, y: 10pt),
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
```c
typedef struct Shape {
    size_t rows;
    size_t cols;
} Shape;
```
]

#pagebreak()

At first, I naively wrote:

#block(
  width: 100%,
  inset: (x: 12pt, y: 10pt),
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
```c
for (int k = 0; k < size; k++) {
    Image img = dataset->index(dataset, k);
    img.label(img, readbyte(fl));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            img.set(img, i, j, readbyte(fi));
        }
    }
}
```
]

Yeah. Per-byte file reads. `readbyte()` is called `60000 × 784 ≈ 47 M` times. I was paying ~47 M function calls. The whole image block is contiguous on disk. it can be read in one shot:

#block(
  width: 100%,
  inset: (x: 12pt, y: 10pt),
  radius: 6pt,
  stroke: 0.5pt + luma(200),
  clip: true,
)[
```c
fread(dataset->content, 1, size * rows * cols, fi);
fread(dataset->labels, 1, size, fl);
```
]

That replaces the entire `for k / for i / for j` loop and is dramatically faster.

normalize to float in [0,1]. One-hot the labels (or keep as uint8_t and index into the loss).

== Algorithm Description & Complexity Analysis

=== Network

- *Architecture:* 784 → 128 → 64 → 10 with ReLU on hidden layers and softmax on the output.

- Store each layer as W[out][in], b[out], plus buffers for activations and gradients. Allocate once, reuse across iterations.

- Init: He/Xavier for weights, zeros for biases.

=== Forward / backward / update

- *Forward:* matrix-vector (or matrix-matrix once you batch) + activation per layer.
- *Loss:* cross-entropy with softmax — fuse them so the gradient at the output is just softmax(z) - y_onehot.
- *Backward:* chain rule layer by layer; each layer needs its input activation (saved during forward) to compute dW.
- *Update:* vanilla SGD $ W -= "lr" * "dW" / "batch_size" $.

=== Training loop

- Shuffle indices each epoch, iterate mini-batches (e.g. 64 or 128), forward → backward → update.
- Track loss + accuracy on the test set per epoch.

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
