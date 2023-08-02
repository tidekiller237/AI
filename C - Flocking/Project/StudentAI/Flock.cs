using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using FullSailAFI.SteeringBehaviors.Core;

namespace FullSailAFI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region Constructors
        public Flock()
        {
        }
        #endregion

        #region TODO Suggested helper methods

        private void CalculateAverages()
        {
            AveragePosition = new Vector3();
            AverageForward = new Vector3();

            foreach(MovingObject boid in Boids)
            {
                AveragePosition += boid.Position;
                AverageForward += boid.Velocity;
            }

            AveragePosition /= Boids.Count();
            AverageForward /= Boids.Count();

            return;
        }

        private Vector3 CalculateAlignmentAcceleration(MovingObject boid)
        {
            Vector3 res = AverageForward / boid.MaxSpeed;
            if(res.Length > 1)
                res.Normalize();
            return res * AlignmentStrength;
        }

        private Vector3 CalculateCohesionAcceleration(MovingObject boid)
        {
            Vector3 res = AveragePosition - boid.Position;
            float dist = res.Length;
            res.Normalize();
            if(dist < FlockRadius)
            {
                res *= dist / FlockRadius;
            }
            return res * CohesionStrength;
        }

        private Vector3 CalculateSeparationAcceleration(MovingObject boid)
        {
            Vector3 res = new Vector3();
            foreach(MovingObject b in Boids)
            {
                Vector3 vec = boid.Position - b.Position;
                float dist = vec.Length;
                float thresh = boid.SafeRadius + b.SafeRadius;
                if(dist < thresh)
                {
                    vec.Normalize();
                    vec *= (thresh - dist) / thresh;
                    res += vec;
                }
            }
            if (res.Length > 1)
                res.Normalize();
            return res * SeparationStrength;
        }

        #endregion

        #region TODO

        public virtual void Update(float deltaTime)
        {
            //calcuate average
            CalculateAverages();

            //calculate boid updates
            foreach (MovingObject boid in Boids)
            {
                Vector3 sum = CalculateAlignmentAcceleration(boid);
                sum += CalculateCohesionAcceleration(boid);
                sum += CalculateSeparationAcceleration(boid);
                sum *= boid.MaxSpeed * deltaTime;
                boid.Velocity += sum;
                if(boid.Velocity.Length > boid.MaxSpeed)
                {
                    boid.Velocity.Normalize();
                    boid.Velocity *= boid.MaxSpeed;
                }
                boid.Update(deltaTime);
            }

            return;
        }
        #endregion
    }
}
