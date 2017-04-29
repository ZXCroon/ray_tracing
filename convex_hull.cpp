//
// Created by zxcroon on 17-4-29.
//

#include "convex_hull.h"

Mesh getConvexHull(vector<gPoint> vertices) {
  assert(vertices.size() >= 4);

  // initialize
  int N = vertices.size();
  orderVector ***edge = new orderVector**[N];
  for (int i = 0; i < N; ++i) {
    // inside?
    edge[i] = new orderVector*[N];
    for (int j = 0; j < N; ++j) {
      edge[i][j] = NULL;
    }
  }
  std::set<orderVector*> facets;

  if (dot(cross(vertices[1] - vertices[0], vertices[2] - vertices[1]), vertices[3] - vertices[0]) > 0) {
    orderVector *f0 = new orderVector{0, 2, 1};
    orderVector *f1 = new orderVector{0, 1, 3};
    orderVector *f2 = new orderVector{1, 2, 3};
    orderVector *f3 = new orderVector{0, 3, 2};
    facets.insert(f0);
    facets.insert(f1);
    facets.insert(f2);
    facets.insert(f3);

    edge[0][1] = f1;
    edge[1][0] = f0;
    edge[0][2] = f0;
    edge[2][0] = f3;
    edge[0][3] = f3;
    edge[3][0] = f1;
    edge[1][2] = f2;
    edge[2][1] = f0;
    edge[1][3] = f1;
    edge[3][1] = f2;
    edge[2][3] = f2;
    edge[3][2] = f3;
  }
  else {
    orderVector *f0 = new orderVector{0, 1, 2};
    orderVector *f1 = new orderVector{0, 3, 1};
    orderVector *f2 = new orderVector{1, 3, 2};
    orderVector *f3 = new orderVector{0, 2, 3};
    facets.insert(f0);
    facets.insert(f1);
    facets.insert(f2);
    facets.insert(f3);

    edge[0][1] = f0;
    edge[1][0] = f1;
    edge[0][2] = f3;
    edge[2][0] = f0;
    edge[0][3] = f1;
    edge[3][0] = f3;
    edge[1][2] = f0;
    edge[2][1] = f2;
    edge[1][3] = f2;
    edge[3][1] = f1;
    edge[2][3] = f3;
    edge[3][2] = f2;
  }

  std::set <std::pair<int, int> > halfEdge;
  for (int i = 4; i < N; ++i) {
    halfEdge.clear();
    for (std::set<orderVector*>::iterator it = facets.begin(); it != facets.end(); ){
      orderVector facet = **it;
      std::cout << cross(vertices[facet[1]] - vertices[facet[0]], vertices[facet[2]] - vertices[facet[1]]) << std::endl;
      if (dot(vertices[i] - vertices[facet[0]], cross(vertices[facet[1]] - vertices[facet[0]],
                                                      vertices[facet[2]] - vertices[facet[1]])) > 0) {
        edge[facet[0]][facet[1]] = NULL;
        edge[facet[1]][facet[2]] = NULL;
        edge[facet[2]][facet[0]] = NULL;
        for (int k = 0; k < 3; ++k) {
          bool have = false;
          for (std::set<std::pair<int, int> >::iterator it1 = halfEdge.begin(); it1 != halfEdge.end();) {
            if (it1->first == facet[(k + 1) % 3] && it1->second == facet[k]) {
              halfEdge.erase(it1++);
              have = true;
              break;
            } else {
              ++it1;
            }
          }
          if (!have) {
            halfEdge.insert(std::pair<int, int>(facet[k], facet[(k + 1) % 3]));
          }
        }

        delete *it;
        facets.erase(it++);

      }
      else {
        ++it;
      }
    }

    for (std::set<std::pair<int, int> >::iterator it1 = halfEdge.begin(); it1 != halfEdge.end(); ++it1) {
      orderVector *newFacet = new orderVector{it1->first, it1->second, i};
      facets.insert(newFacet);
      edge[it1->first][it1->second] = edge[it1->second][i] = edge[i][it1->first] = newFacet;
    }
  }

  // rearrange
  vector<gPoint> vList;
  int newN = 0;
  int *newOrder = new int[N];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (edge[i][j]) {
        newOrder[i] = newN;
        ++newN;
        vList.push_back(vertices[i]);
        break;
      }
    }
  }
  vector<orderVector> fList;
  for (std::set<orderVector*>::iterator it = facets.begin(); it != facets.end(); ++it) {
    fList.push_back(orderVector{newOrder[(**it)[0]], newOrder[(**it)[1]], newOrder[(**it)[2]]});
  }

  // release
  for (int i = 0; i < N; ++i) {
    delete[] edge[i];
  }
  delete[] edge;

  return Mesh(vList, fList);
}
