// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include <iostream>

#include "datatypes.h"
#include "library_float.h"
#include "profile.h"

// C = A + B
void MatAdd(float* A, float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = A[i * J + j];
			float b = B[i * J + j];

			float c = a + b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = a + B
void MatAddBroadCastA(float* A, float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = *A;
			float b = B[i * J + j];

			float c = a + b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = A + b
void MatAddBroadCastB(float* A, float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = A[i * J + j];
			float b = *B;

			float c = a + b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = A - B
void MatSub(float* A, const float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = A[i * J + j];
			float b = B[i * J + j];

			float c = a - b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = a - B
void MatSubBroadCastA(float* A, const float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = *A;
			float b = B[i * J + j];

			float c = a - b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = A - b
void MatSubBroadCastB(float* A, const float* B, float* C, MYINT I, MYINT J, MYINT shrA, MYINT shrB, MYINT shrC) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = A[i * J + j];
			float b = *B;

			float c = a - b;

			C[i * J + j] = c;
		}
	}
	return;
}

// C = A * B
void MatMulNN(float *A, float *B, float *C, float *tmp, MYINT I, MYINT K, MYINT J, MYINT shrA, MYINT shrB, MYINT H1, MYINT H2) {

	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			for (MYINT k = 0; k < K; k++) {
				float a = A[i * K + k];
				float b = B[k * J + j];

				tmp[k] = a * b;
			}

			MYINT count = K, depth = 0;
			bool shr = true;

			while (depth < (H1 + H2)) {
				if (depth >= H1)
					shr = false;

				for (MYINT p = 0; p < (K / 2 + 1); p++) {
					float sum;
					if (p < (count >> 1))
						sum = tmp[2 * p] + tmp[(2 * p) + 1];
					else if ((p == (count >> 1)) && ((count & 1) == 1))
						sum = tmp[2 * p];
					else
						sum = 0;

					if (shr)
						tmp[p] = sum;
					else
						tmp[p] = sum;
				}
				count = (count + 1) >> 1;

				depth++;
			}

			C[i * J + j] = tmp[0];
		}
	}
	return;
}

// C = A * B
void MatMulCN(const float *A, float *B, float *C, float *tmp, MYINT I, MYINT K, MYINT J, MYINT shrA, MYINT shrB, MYINT H1, MYINT H2) {

	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			for (MYINT k = 0; k < K; k++) {
				float a = A[i * K + k];
				float b = B[k * J + j];

				tmp[k] = a * b;
			}

			MYINT count = K, depth = 0;
			bool shr = true;

			while (depth < (H1 + H2)) {
				if (depth >= H1)
					shr = false;

				for (MYINT p = 0; p < (K / 2 + 1); p++) {
					float sum;
					if (p < (count >> 1))
						sum = tmp[2 * p] + tmp[(2 * p) + 1];
					else if ((p == (count >> 1)) && ((count & 1) == 1))
						sum = tmp[2 * p];
					else
						sum = 0;

					if (shr)
						tmp[p] = sum;
					else
						tmp[p] = sum;
				}
				count = (count + 1) >> 1;

				depth++;
			}

			C[i * J + j] = tmp[0];
		}
	}
	return;
}

// C = A * B
void MatMulNC(float *A, const float *B, float *C, float *tmp, MYINT I, MYINT K, MYINT J, MYINT shrA, MYINT shrB, MYINT H1, MYINT H2) {

	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			for (MYINT k = 0; k < K; k++) {
				float a = A[i * K + k];
				float b = B[k * J + j];

				tmp[k] = a * b;
			}

			MYINT count = K, depth = 0;
			bool shr = true;

			while (depth < (H1 + H2)) {
				if (depth >= H1)
					shr = false;

				for (MYINT p = 0; p < (K / 2 + 1); p++) {
					float sum;
					if (p < (count >> 1))
						sum = tmp[2 * p] + tmp[(2 * p) + 1];
					else if ((p == (count >> 1)) && ((count & 1) == 1))
						sum = tmp[2 * p];
					else
						sum = 0;

					if (shr)
						tmp[p] = sum;
					else
						tmp[p] = sum;
				}
				count = (count + 1) >> 1;

				depth++;
			}

			C[i * J + j] = tmp[0];
		}
	}
	return;
}

// C = A * B
void MatMulCC(const float *A, const float *B, float *C, float *tmp, MYINT I, MYINT K, MYINT J, MYINT shrA, MYINT shrB, MYINT H1, MYINT H2) {

	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			for (MYINT k = 0; k < K; k++) {
				float a = A[i * K + k];
				float b = B[k * J + j];

				tmp[k] = a * b;
			}

			MYINT count = K, depth = 0;
			bool shr = true;

			while (depth < (H1 + H2)) {
				if (depth >= H1)
					shr = false;

				for (MYINT p = 0; p < (K / 2 + 1); p++) {
					float sum;
					if (p < (count >> 1))
						sum = tmp[2 * p] + tmp[(2 * p) + 1];
					else if ((p == (count >> 1)) && ((count & 1) == 1))
						sum = tmp[2 * p];
					else
						sum = 0;

					if (shr)
						tmp[p] = sum;
					else
						tmp[p] = sum;
				}
				count = (count + 1) >> 1;

				depth++;
			}

			C[i * J + j] = tmp[0];
		}
	}
	return;
}

// C = A |*| B
void SparseMatMul(const MYINT *Aidx, const float *Aval, float **B, float *C, MYINT K, MYINT shrA, MYINT shrB, MYINT shrC) {

	MYINT ite_idx = 0, ite_val = 0;
	for (MYINT k = 0; k < K; k++) {
		// float b = getIntFeature(k);
		float b = B[k * 1][0];

		MYINT idx = Aidx[ite_idx];
		while (idx != 0) {
			float a = Aval[ite_val];

			float c = a * b;

			C[idx - 1] += c;

			ite_idx++;
			ite_val++;

			idx = Aidx[ite_idx];
		}
		ite_idx++;
	}

	return;
}

// C = A <*> B
void MulCir(float *A, float *B, float *C, MYINT I, MYINT J, MYINT shrA, MYINT shrB) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float a = A[i * J + j];
			float b = B[i * J + j];

			C[i * J + j] = a * b;
		}
	}
	return;
}

// A = tanh(A)
void TanH(float *A, MYINT I, MYINT J, float tanh_limit) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float x = A[i * J + j], y;

			if (x >= tanh_limit)
				y = tanh_limit;
			else if (x <= -tanh_limit)
				y = -tanh_limit;
			else
				y = x;

			A[i * J + j] = y;
		}
	}
	return;
}

// index = argmax(A)
void ArgMax(float *A, MYINT I, MYINT J, MYINT *index) {

	float max = A[0];
	MYINT maxIndex = 0;
	MYINT counter = 0;
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float x = A[i * J + j];

			if (max < x) {
				maxIndex = counter;
				max = x;
			}

			counter++;
		}
	}

	*index = maxIndex;

	return;
}

// A = A^T
void Transpose(float *A, float *B, MYINT I, MYINT J) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			B[i * J + j] = A[j * I + i];
		}
	}
	return;
}

// C = a * B
void ScalarMul(float *A, float *B, float *C, MYINT I, MYINT J, MYINT shrA, MYINT shrB) {

	float a = *A;

	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float b = B[i * J + j];

			C[i * J + j] = a * b;
		}
	}

	return;
}

// C = A # B
// A[N][H][W][CI], B[HF][WF][CI][CO], C[N][H][W][CO]
void Conv(float *A, const float *B, float *C, float *tmp, MYINT N, MYINT H, MYINT W, MYINT CI, MYINT HF, MYINT WF, MYINT CO, MYINT shrA, MYINT shrB, MYINT H1, MYINT H2) {
	MYINT padH = (HF - 1) / 2;
	MYINT padW = (WF - 1) / 2;

	for (MYINT n = 0; n < N; n++) {
		for (MYINT h = 0; h < H; h++) {
			for (MYINT w = 0; w < W; w++) {
				for (MYINT co = 0; co < CO; co++) {

					MYINT counter = 0;
					for (MYINT hf = 0; hf < HF; hf++) {
						for (MYINT wf = 0; wf < WF; wf++) {
							for (MYINT ci = 0; ci < CI; ci++) {
								float a = (((((h + hf) < padH) || ((h + hf) >= (H + padH))) || (((w + wf) < padW) || ((w + wf) >= (W + padW)))) ? 0 : A[n * H * W * CI + ((h + hf) - padH) * W * CI + ((w + wf) - padW) * CI + ci]);

								float b = B[hf * WF * CI * CO + wf * CI * CO + ci * CO + co];

								tmp[counter] = a * b;
								counter++;
							}
						}
					}

					MYINT totalEle = HF * WF * CI;
					MYINT count = HF * WF * CI, depth = 0;
					bool shr = true;

					while (depth < (H1 + H2)) {
						if (depth >= H1)
							shr = false;

						for (MYINT p = 0; p < (totalEle / 2 + 1); p++) {
							float sum;
							if (p < (count >> 1))
								sum = tmp[2 * p] + tmp[(2 * p) + 1];
							else if ((p == (count >> 1)) && ((count & 1) == 1))
								sum = tmp[2 * p];
							else
								sum = 0;

							if (shr)
								tmp[p] = sum;
							else
								tmp[p] = sum;
						}
						count = (count + 1) >> 1;

						depth++;
					}

					C[n * H * W * CO + h * W * CO + w * CO + co] = tmp[0];
				}
			}
		}
	}

	return;
}

// A = A <+> B
// A[N][H][W][C], B[C]
void AddOrSubCir4D(float *A, const float *B, MYINT N, MYINT H, MYINT W, MYINT C, MYINT shrA, MYINT shrB, MYINT shrC, bool add) {

	for (MYINT n = 0; n < N; n++) {
		for (MYINT h = 0; h < H; h++) {
			for (MYINT w = 0; w < W; w++) {
				for (MYINT c = 0; c < C; c++) {
					float a = A[n * H * W * C + h * W * C + w * C + c];

					float b = B[c];

					float res;
					if (add)
						res = a + b;
					else
						res = a - b;

					A[n * H * W * C + h * W * C + w * C + c] = res;
				}
			}
		}
	}

	return;
}

// A = A <+> B
// A[N][H][W][C], B[C]
void AddOrSubCir2D(float *A, const float *B, MYINT H, MYINT W, MYINT shrA, MYINT shrB, MYINT shrC, bool add) {

	for (MYINT h = 0; h < H; h++) {
		for (MYINT w = 0; w < W; w++) {
			float a = A[h * W + w];

			float b = B[w];

			float res;
			if (add)
				res = a + b;
			else
				res = a - b;

			A[h * W + w] = res;
		}
	}

	return;
}

// A = relu(A)
// A[N][H][W][C]
void Relu4D(float *A, MYINT N, MYINT H, MYINT W, MYINT C) {

	for (MYINT n = 0; n < N; n++) {
		for (MYINT h = 0; h < H; h++) {
			for (MYINT w = 0; w < W; w++) {
				for (MYINT c = 0; c < C; c++) {
					float a = A[n * H * W * C + h * W * C + w * C + c];
					if (a < 0)
						a = 0;

					A[n * H * W * C + h * W * C + w * C + c] = a;
				}
			}
		}
	}

	return;
}

// A = relu(A)
// A[N][H][W][C]
void Relu2D(float *A, MYINT H, MYINT W) {

	for (MYINT h = 0; h < H; h++) {
		for (MYINT w = 0; w < W; w++) {
			float a = A[h * W + w];
			if (a < 0)
				a = 0;

			A[h * W + w] = a;
		}
	}

	return;
}

// B = maxpool(A)
// A[N][H][W][C], B[N][H][W][C]
void Maxpool(float *A, float *B, MYINT N, MYINT H, MYINT W, MYINT C, MYINT stride) {
	MYINT HO = H / stride;
	MYINT WO = W / stride;

	for (MYINT n = 0; n < N; n++) {
		for (MYINT ho = 0; ho < HO; ho++) {
			for (MYINT wo = 0; wo < WO; wo++) {
				for (MYINT c = 0; c < C; c++) {

					float max = A[n * H * W * C + (stride * ho) * W * C + (stride * wo) * C + c];
					for (MYINT hs = 0; hs < stride; hs++) {
						for (MYINT ws = 0; ws < stride; ws++) {
							float a = A[n * H * W * C + ((stride * ho) + hs) * W * C + ((stride * wo) + ws) * C + c];
							if (a > max)
								max = a;
						}
					}

					B[n * HO * WO * C + ho * WO * C + wo * C + c] = max;
				}
			}
		}
	}

	return;
}

// B = exp(A)
void Exp(float *A, MYINT I, MYINT J, MYINT shrA, MYINT shrB, float *B) {

	for (MYITE i = 0; i < I; i++) {
		for (MYITE j = 0; j < J; j++) {
			float x = A[i * J + j];

			updateRangeOfExp(-x);

			B[i * J + j] = exp(x);
		}
	}

	return;
}

// A = sigmoid(A)
void Sigmoid(float* A, MYINT I, MYINT J) {
	for (MYINT i = 0; i < I; i++) {
		for (MYINT j = 0; j < J; j++) {
			float x = A[i * J + j], y;

			y = 1 / (1 + exp(-x));

			A[i * J + j] = y;
		}
	}
	return;
}
