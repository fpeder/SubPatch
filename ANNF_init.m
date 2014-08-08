function nn = ANNF_init(a, b, w, s, params)
% function nn = ANNF_init(a, b, w, s, params)
% a        target image
% b        source image
% w        patch width
% s        downsampling factor 
% params   rd-tree config params
% -> nn    initial ANNF
    
    [M, N, ~] = size(a);
    [pa,  ~] = mexImgToPvec2(a, w, s);
    [pb, xy] = mexImgToPvec2(b, w, s);
    [idx, d] = flann_search(single(pb'), single(pa'), 1, params);
    
    M = floor((M-w)/(s+1)) + 1;
    N = floor((N-w)/(s+1)) + 1;

    x = reshape(xy(idx, 1), [N, M])';
    y = reshape(xy(idx, 2), [N, M])';
    d = reshape(d, [N, M])';

    nn = zeros(M, N, 3, 'int32');
    nn(:, :, 1) = x;
    nn(:, :, 2) = y;
    nn(:, :, 3) = d;
    
end
