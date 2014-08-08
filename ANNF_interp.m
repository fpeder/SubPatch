function out = ANNF_interp(nn, d)
% function out = ANNF_inter(nn, d)
    
    out = zeros(size(nn, 1)*(d+1), size(nn, 2)*(d+1), 3, 'int32');
    ker = int32(ones(d+1, d+1));
    
    for i = 1:3
        out(:, :, i) = kron(nn(:,:,i), ker);
    end

end
