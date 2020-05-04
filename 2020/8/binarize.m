function B = binarize(A)
    [fr,v] = hist(A(:),10);
    B=(A<=v(1));
