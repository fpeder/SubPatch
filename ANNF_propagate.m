function nn = ANNF_propagate(nn, a, b, w, niter)
% function nn = ANNF_propagate(nn, a, b, w, niter)
% a      target image
% b      source image
% w      patch width 
% niter  #propagation
% -> nn  ANNF

    xy = int32(nn(:, :, 1:2));
    d = int32(nn(:, :, 3));

    nn = zeros(size(d, 1), size(d, 2), 3, 'int32');
    [p q] = mexNnImp(xy, d, a, b, w, niter);
    
    nn(:, :, 1:2) = p;
    nn(:, :, 3) = q;

end
