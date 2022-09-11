

void matrix_mul(float *a, float *b, float *c){
    float m_a[4][4];
    float m_b[4][4];
    float m_c[4][4]; // m_c = m_a * m_b

    int n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            m_a[i][j] = a[n];
            m_b[i][j] = b[n];
            n += 1;
        }
    }

    for(int i=0;i<4;i++){    
        for(int j=0;j<4;j++){
            m_c[i][j]=0.0;
            for(int k=0; k < 4; k++){
                m_c[i][j] += m_a[i][k]*m_b[k][j];    
            }
        }
    }

    n = 0;
    for(int i=0; i < 4; i++){
        for(int j=0; j < 4; j++){
            c[n] = m_c[i][j];
            n += 1;
        }
    }
}