#!/bin/bash
set -e
make clean
make
#edit by bmj
# Makes programs, downloads sample data, trains a GloVe model, and then evaluates it.
# One optional argument can specify the language used for eval script: matlab, octave or [default] python
ALLCORPUS=text2g_add_1g
CORPUS=text2g
CORPUS_DELTA=text_new_1g
VOCAB_FILE=vocab.txt
COOCCURRENCE_FILE=cooccurrence.bin
COOCCURRENCE_DELTA_FILE=cooccurrencedelta.bin
COOCCURRENCE_SHUF_FILE=shufresult.bin
ORIGIN_SHUF_FILE=shuforigin.bin
RESULT=result.bin
SHUFULLERESULT=shufresult.bin
INITVECTORS=vectors1.bin
INITGRAD=gradsq1.bin
BUILDDIR=build
SAVE_FILE=vectors
SAVE_VECTORS=vectors1
SAVE_VECTORS_GLOBAL=vectors2
SAVE_GRAD=gradsq1
VERBOSE=2
MEMORY=4.0
VOCAB_MIN_COUNT=5
VECTOR_SIZE=300
MAX_ITER=25
MAX_ITER2=30
MAX_ITER_INCREMENTAL=5
WINDOW_SIZE=15
BINARY=2
NUM_THREADS=8
X_MAX=100
X_MAX0=130
GRADSQ_FILE=gradsq
MODEL=0
ORIGIN=2
# this vocab_count helps you to establish the vocab_hash
echo "$ $BUILDDIR/vocab_count -min-count $VOCAB_MIN_COUNT -verbose $VERBOSE < $ALLCORPUS > $VOCAB_FILE"
$BUILDDIR/vocab_count -min-count $VOCAB_MIN_COUNT -verbose $VERBOSE < $ALLCORPUS > $VOCAB_FILE
#this coocur helps you to establish the cooccur file of the text
echo "$ $BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $CORPUS > $COOCCURRENCE_FILE"
$BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $CORPUS > $COOCCURRENCE_FILE
#shuffle the cooccur file
echo "$ $BUILDDIR/shuffle -memory $MEMORY -verbose $VERBOSE <$COOCCURRENCE_FILE >$ORIGIN_SHUF_FILE"
$BUILDDIR/shuffle -memory $MEMORY -verbose $VERBOSE <$COOCCURRENCE_FILE  >$ORIGIN_SHUF_FILE

echo "now calculate ORIGIN"
#calculate the origin gradsq to help you build the increamental
echo "$ $BUILDDIR/glove2 -save-file $SAVE_VECTORS -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX -iter $MAX_ITER -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL"
$BUILDDIR/glove2 -save-file $SAVE_VECTORS -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX -iter $MAX_ITER -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL

#echo "$ python eval/python/evaluate.py"
#python eval/python/evaluate.py

#calc the delta file cooccur
echo "$ $BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $CORPUS_DELTA > $COOCCURRENCE_DELTA_FILE"
$BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $CORPUS_DELTA > $COOCCURRENCE_DELTA_FILE
#combine the delta file with the origin file
echo "$ $BUILDDIR/combine -cooccurrence-file $COOCCURRENCE_FILE -cooccurrencedelta-file $COOCCURRENCE_DELTA_FILE -result-file $RESULT"
$BUILDDIR/combine -cooccurrence-file $COOCCURRENCE_FILE -cooccurrencedelta-file $COOCCURRENCE_DELTA_FILE -result-file $RESULT

#help shuffle the increamental
echo "$ $BUILDDIR/shuffle2 -memory $MEMORY -verbose $VERBOSE <$RESULT >$SHUFULLERESULT"
$BUILDDIR/shuffle2 -memory $MEMORY -verbose $VERBOSE <$RESULT  >$SHUFULLERESULT

echo "now calculate Increamental"

#calc the increamental vectors.txt
echo "$ $BUILDDIR/glove2 -save-file $SAVE_FILE -threads $NUM_THREADS -input-file $COOCCURRENCE_SHUF_FILE -x-max $X_MAX -x-max0 $X_MAX0  -iter $MAX_ITER_INCREMENTAL -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $GRADSQ_FILE -model $MODEL -initgrad $INITGRAD -initvectors $INITVECTORS -origin $ORIGIN"
$BUILDDIR/glove2 -save-file $SAVE_FILE -threads $NUM_THREADS -input-file $COOCCURRENCE_SHUF_FILE -x-max $X_MAX -x-max0 $X_MAX0 -iter $MAX_ITER_INCREMENTAL -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $GRADSQ_FILE -model $MODEL -initgrad $INITGRAD -initvectors $INITVECTORS -origin $ORIGIN

#echo "$ python eval/python/evaluate.py"
#python eval/python/evaluate.py

#all file
echo "$ $BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $ALLCORPUS > $COOCCURRENCE_FILE"
$BUILDDIR/cooccur -memory $MEMORY -vocab-file $VOCAB_FILE -verbose $VERBOSE -window-size $WINDOW_SIZE < $ALLCORPUS > $COOCCURRENCE_FILE
#shuffle
echo "$ $BUILDDIR/shuffle -memory $MEMORY -verbose $VERBOSE <$COOCCURRENCE_FILE >$ORIGIN_SHUF_FILE"
$BUILDDIR/shuffle -memory $MEMORY -verbose $VERBOSE <$COOCCURRENCE_FILE  >$ORIGIN_SHUF_FILE

echo "now calculate Global"
#vectors2.txt
echo "$ $BUILDDIR/glove2 -save-file $SAVE_VECTORS_GLOBAL -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX0 -iter $MAX_ITER -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL"
$BUILDDIR/glove2 -save-file $SAVE_VECTORS_GLOBAL -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX0 -iter $MAX_ITER2 -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL
#vectors1.txt
$BUILDDIR/shuffle -memory $MEMORY -verbose $VERBOSE <$COOCCURRENCE_FILE  >$ORIGIN_SHUF_FILE
echo "$ $BUILDDIR/glove2 -save-file $SAVE_VECTORS -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX0 -iter $MAX_ITER -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL"
$BUILDDIR/glove2 -save-file $SAVE_VECTORS -threads $NUM_THREADS -input-file $ORIGIN_SHUF_FILE -x-max $X_MAX0 -iter $MAX_ITER -vector-size $VECTOR_SIZE -binary $BINARY -vocab-file $VOCAB_FILE -verbose $VERBOSE -gradsq-file $SAVE_GRAD -model $MODEL

echo "run MSE test"
$BUILDDIR/mse 
