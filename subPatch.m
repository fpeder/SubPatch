function nn = subPatch(a, b, w, d, niter, varargin)
% function nn = subPatch(a, b, w, d, niter, params)
% a         target image
% b         source image
% w         patch width
% d         downsampling factor
% niter     #propgations
% optional  rd-tree config params
% -> nn     ANNF 

    if nargin == 6 
        params = varargin{1};
    else 
        params.algorithm = 'kdtree';
        params.tree = 4;
        params.check = 8;
        params.core = 4;
    end
    
    nn = ANNF_init(a, b, w, d, params);
    nn = ANNF_interp(nn, d);
    nn = ANNF_propagate(nn, a, b, w, niter);

end
