/**
 * @file   gaussjordan.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:13:11 2009
 *
 * @brief  Implementation of Gauss-Jordan-Algorithm
 *
 *
 */

#include <core/cxsccomp.hpp>
#include <algorithm/opt-lib/gaussjordan.hpp>

namespace GlobOpt {


    /*!
  ** Swaps two rows in a matrix
  **
  ** @param mat matrix
  ** @param i first row to swap
  ** @param j second row to swap
  */
  void swapRows(core::arith::rmatrix &mat, int i, int j)
  {
    //DONE_C: Neuimplementierung fuer mtl
    core::arith::rvector v(num_cols(mat));
    for(unsigned k = 0; k < num_cols(mat); k++)
      v[i] = mat[i][k];

    for(unsigned k = 0; k < num_cols(mat);k++)
      mat[i][k] = mat[j][k];

    for(unsigned k = 0; k < num_rows(v);k++)
      mat[j][k] = v[k];
  }



  /*!
  ** Finds the next col in the matrix for the Gauss-Jordan algorithm
  **
  ** @param lhs left hand side
  ** @param rhs right hand side
  ** @param row starting row
  ** @param col starting column
  **
  ** @return index of next column with a nonzero pivot or -1 if no such
  ** row exists and the matrix is singular
  */
// suche naechste spalte, mit einem eintrag der nicht 0 ist
// row = startzeile. col = startspalte
  int nextCol(core::arith::rmatrix &lhs, core::arith::rmatrix &rhs, int row, int col)
  {
    int i, j;
    int found = -1;
    core::arith::mreal Tiny = 1E-200;


    for(i = col; i < core::arith::Ub(lhs,2) && found == -1; i++) {
      for(j = row; j < core::arith::Ub(lhs, 1); j++) {
        if(abs(lhs[j][i]) > Tiny) {
          found = i;
          break;
        }
      }
    }

    // swap erforderlich
    if(j != row && found != -1) {
      swapRows(lhs, row, j);
      swapRows(rhs, row, j);
    }

    return found;
  }

  bool gjelim(core::arith::rmatrix &lhs, core::arith::rmatrix &rhs) {
    assert(core::arith::Ub(lhs, 1) == core::arith::Ub(rhs,1));

    int col = 0;
    int row = 0;

    while(col < core::arith::Ub(lhs, 2) && row < core::arith::Ub(lhs, 1)) {
      // 1, 2
      col = nextCol(lhs, rhs, row, col);

      // kein pivot != 0 gefunden, abbrechen!
      if(col == -1)
        break;

      // 3 (erste zeile durch pivot)
      core::arith::mreal pivot = lhs[row][col];
      for(int i = col; i < core::arith::Ub(lhs, 2); i++)
        lhs[row][i] /= pivot;
      for(int i = 0; i < core::arith::Ub(rhs, 2); i++)
        rhs[row][i] /= pivot;


      // 4
      // fuer uebrige zeilen
      for(int i = row; i < core::arith::Ub(lhs, 1); i++) {
        core::arith::mreal r = lhs[i][col];
        // fuer jede spalte
        for(int j = col;r != 0 && j < core::arith::Ub(lhs, 2); j++)
          lhs[i][j] -= r*lhs[row][j];
        for(int j = 0; r != 0 && j < core::arith::Ub(rhs, 2); j++)
          rhs[i][j] -= r*rhs[row][j];
      }

      // 5
      col++;
      row++;
    }

    // 6
    if(col != -1) {
      // lhs nun in Zeilenstufenform
      // beginne mit letzter zeile
      for(int k = core::arith::Ub(lhs, 1)-1; k >= core::arith::Lb(lhs, 1); k--) {
        // fuer die oberen zeilen
        for(int i = k - 1; i >= core::arith::Lb(lhs, 1); i--) {
          // tue mit jeder spalte
          core::arith::mreal r = lhs[i][k];
          for(int j = 0; j < core::arith::Ub(lhs, 2); j++)
            lhs[i][j] -= r*lhs[k][j];
          for(int j = 0; j < core::arith::Ub(rhs, 2); j++)
            rhs[i][j] -= r*rhs[k][j];
        }
      }
    }

    return col != -1;
  }

  core::arith::rmatrix invertMatrix(const core::arith::rmatrix &m, bool &sucess)
  {
    core::arith::rmatrix lhs(m);
    core::arith::rmatrix rhs(core::arith::Id(lhs));
    sucess = gjelim(lhs, rhs);
    return rhs;
  }

}
