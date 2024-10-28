#!/bin/bash

direktorijum=$1
find $direktorijum maxdepth 1 -type f -size +0c | wc -l  -- u direktorijumu koji se zadaje kao ulazni argument odredjuje broj datoteka čija je veličina veda od 0B. 
