function err = rmse(X, Y)
% function err = rmse(X, Y)
    
    [M, N, n] = size(X);
    X = double(X);
    Y = double(Y);

    err = 0;
    for i = 1:n
        tmp = sum(sum( (X(:,:,i) - Y(:, :, i)).^2 ));
        err = err + sqrt(1/(M*N) * tmp);
    end

end
