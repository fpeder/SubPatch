function out = ANNF_interp(nn, s)
% function out = ANNF_inter(nn, s)
% nn      scaled ANNF
% s       upsampling factor
% -> out  interpolated ANNF
    
    out = zeros(size(nn, 1)*(s+1), size(nn, 2)*(s+1), 3, 'int32');
    ker = int32(ones(s+1, s+1));
    
    for i = 1:3
        out(:, :, i) = kron(nn(:,:,i), ker);
    end

end
