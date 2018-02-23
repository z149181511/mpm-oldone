#ifndef MPM_NODE_H_
#define MPM_NODE_H_

#include <array>
#include <iostream>
#include <limits>
#include <vector>

#include "node_base.h"
#include "serialize.h"

namespace mpm {

//! Global index type for the node
using Index = unsigned long long;

// Node base class
//! \brief Base class that stores the information about nodes
//! \details Node class: id_ and coordinates.
//! \tparam Tdim Dimension
//! \tparam Tdof Degrees of Freedom
//! \tparam Tnphases Number of phases
template <unsigned Tdim, unsigned Tdof, unsigned Tnphases>
class Node : public NodeBase<Tdim> {
 public:
  //! Define a vector of size dimension
  using VectorDim = Eigen::Matrix<double, Tdim, 1>;

  //! Constructor with id, coordinates and dof
  Node(Index id, const VectorDim& coord);

  //! Destructor
  virtual ~Node(){};

  //! Delete copy constructor
  Node(const Node<Tdim, Tdof, Tnphases>&) = delete;

  //! Delete assignement operator
  Node& operator=(const Node<Tdim, Tdof, Tnphases>&) = delete;

  //! Initialise properties
  void initialise();

  //! Return degrees of freedom
  unsigned dof() const { return dof_; }

  //! Assign nodal mass
  void assign_mass(unsigned nphase, double mass) { mass_(0, nphase) = mass; }

  //! Return mass
  double mass(unsigned nphase) const { return mass_(0, nphase); }

  //! Assign force
  void assign_force(unsigned nphase, const Eigen::VectorXd& force);

  //! Return force
  Eigen::VectorXd force(unsigned nphase) const { return force_.col(nphase); }

  //! Assign velocity
  void assign_velocity(unsigned nphase, const Eigen::VectorXd& velocity);

  //! Return velocity
  Eigen::VectorXd velocity(unsigned nphase) const {
    return velocity_.col(nphase);
  }

  //! Assign momentum
  void assign_momentum(unsigned nphase, const Eigen::VectorXd& momentum);

  //! Return momentum
  Eigen::VectorXd momentum(unsigned nphase) const {
    return momentum_.col(nphase);
  }

  //! Assign acceleration
  void assign_acceleration(unsigned nphase,
                           const Eigen::VectorXd& acceleration);

  //! Return acceleration
  Eigen::VectorXd acceleration(unsigned nphase) const {
    return acceleration_.col(nphase);
  }

  //! Update nodal mass
  void update_mass(const Eigen::VectorXd& mass) { mass_ += mass; }

  //! Update nodal momentum
  void update_momentum(const Eigen::MatrixXd& momentum) {
    momentum_ += momentum;
  }

  //! Update body force
  void update_body_force(const Eigen::MatrixXd& body_force) {
    ext_force_ += body_force;
  }

  //! Update traction force
  void update_traction_force(const Eigen::MatrixXd& traction_force) {
    ext_force_ += traction_force;
  }

  //! Update internal force
  void update_internal_force(const Eigen::MatrixXd& internal_force) {
    int_force_ += internal_force;
  }

  //! Compute velocity from the momentum
  void compute_velocity();

 protected:
  //! node id
  using NodeBase<Tdim>::id_;
  //! nodal coordinates
  using NodeBase<Tdim>::coordinates_;

 private:
  //! Degrees of freedom
  unsigned dof_{std::numeric_limits<unsigned>::max()};
  //! Mass solid
  Eigen::Matrix<double, 1, Tnphases> mass_;
  //! Force
  Eigen::Matrix<double, Tdof, Tnphases> force_;
  //! External force
  Eigen::Matrix<double, Tdim, Tnphases> ext_force_;
  //! Internal force
  Eigen::Matrix<double, Tdim, Tnphases> int_force_;
  //! Velocity
  Eigen::Matrix<double, Tdim, Tnphases> velocity_;
  //! Momentum
  Eigen::Matrix<double, Tdim, Tnphases> momentum_;
  //! Acceleration
  Eigen::Matrix<double, Tdim, Tnphases> acceleration_;
};  // Node class
}  // namespace mpm

#include "node.tcc"

#endif  // MPM_NODE_H_
